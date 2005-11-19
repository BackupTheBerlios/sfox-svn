#include "clipmap.h"
#include "mipmap.h"

using namespace StarEngine;

#define DATAPATH "../../common"

struct s_vec3 {
  float x,y,z;
};

struct s_vec4 {
  float x,y,z,w;
};

struct s_vec2 {
  float x,y;
};

ClipMap::ClipMap(int n)
  : clipmapSize(n), wireframe(false)
{
  genBlocks();
  genRingFixUp();
  genFinestLevel();

  //Load float terrain texture
  fprintf(stderr, "Generating mipmap...");
  mipmap = new Mipmap;
  mipmap->buildMipmap(DATAPATH"/media/clipmap/terrain/bigterrain.png", 4);
  fprintf(stderr, "Done\n");

  mipmap->getTextures(geomTex, 1024, 1024, n+1, n+1);

  clipmapVert = new ShaderCG();
  clipmapVert->loadSourceFromFile("shaders/clipmap_vert.cg",
                                  ShaderCG::VERTEX);

  clipmapFrag = new ShaderCG();
  clipmapFrag->loadSourceFromFile("shaders/clipmap_frag.cg",
                                  ShaderCG::FRAGMENT);
  ShaderCG::setManageTextureParameters(true);

  Texture2D *tex = (Texture2D *)g_TextureManager.load("grass", DATAPATH"/media/clipmap/textures/grass.png");
  tex->setMinFilter(TF_LINEAR);
  tex->setMagFilter(TF_LINEAR);
  clipmapFrag->setTextureParameter("grass", tex);

  tex = (Texture2D *)g_TextureManager.load("cliff", DATAPATH"/media/clipmap/textures/cliff_face.jpg");
  tex->setMinFilter(TF_LINEAR);
  tex->setMagFilter(TF_LINEAR);
  clipmapFrag->setTextureParameter("cliff", tex);
}
/****************************************************************************/

void
ClipMap::genBlocks()
{
  int m = (clipmapSize+1)/4;
  blockVertices = new GeometricBatch;
  blockVertices->setPrimitiveMode(PM_TRIANGLE_STRIP);
  blockVertices->setVertexFormat("vertex:float3");
  blockVertices->setVertices(m*m*sizeof(s_vec3), NULL, UT_STATIC_DRAW);
  s_vec3 *vertices = (s_vec3 *)blockVertices->lock(AT_WRITE_ONLY);
  for(int j = 0; j < m; j++)
    for(int i = 0; i < m; i++) {
      vertices[i+j*m].x = i;
      vertices[i+j*m].y = 0;
      vertices[i+j*m].z = j;
    }
  blockVertices->unlock();

  blockIndices = new IndicesBatch();
  int numIndices = (2*m+2)*(m-2)+2*m;
  blockIndices->setIndices(numIndices, SE_UNSIGNED_SHORT, NULL,
                           UT_STATIC_DRAW);
  int ofs = 0;
  unsigned short *ind = (unsigned short *)blockIndices->lock(AT_WRITE_ONLY);
  for(int j = 0; j < m; j++) {
    for(int i = 0; i < m; i++) {
      ind[ofs++] = i+j*m;
      ind[ofs++] = i+(j+1)*m;
    }
    if(j==m-2)
      break;
    ind[ofs++] = m-1+(j+1)*m;
    ind[ofs++] = m-1+j*m;
    j++;

    for(int i = m-1; i >= 0; i--) {
      ind[ofs++] = i+(j+1)*m;
      ind[ofs++] = i+j*m;
    }
    ind[ofs++] = (j+2)*m;
    ind[ofs++] = (j+1)*m;
  }
  assert(numIndices == ofs);
  blockIndices->unlock();
}

/****************************************************************************/
void
ClipMap::genRingFixUp()
{
  int m = (clipmapSize+1)/4;

  ringFixupVertices = new GeometricBatch;
  ringFixupVertices->setPrimitiveMode(PM_TRIANGLE_STRIP);
//  ringFixupVertices->setPrimitiveMode(PM_POINTS);
  ringFixupVertices->setVertexFormat("vertex:float3");
  ringFixupVertices->setVertices(m*3*4*sizeof(s_vec3), NULL, UT_STATIC_DRAW);
  s_vec3 *vertices = (s_vec3 *)ringFixupVertices->lock(AT_READ_WRITE);
  float startx = 2*(m-1);
  for(int k = 0; k < 2; k++) {
    for(int j = 0; j < m; j++)
      for(int i = 0; i < 3; i++) {
        int ofs = i+j*3+k*3*m;
        vertices[ofs].x = startx+i;
        vertices[ofs].y = 0;
        vertices[ofs].z = j+k*(3*(m-1)+2);
      }
  }
  int ofs = 2*3*m;
  for(int i = 0; i < 2*3*m; i++) {
    vertices[ofs].x = vertices[i].z;
    vertices[ofs].y = 0;
    vertices[ofs++].z = vertices[i].x;
  }
  ringFixupVertices->unlock();

  ringFixupIndices = new IndicesBatch();
//  int numIndices = (2*3+2)*(2*m-3)+2*3+ 2 + 3*2*(2*m-1)+(2*m-1)*2-10;
  int numIndices = (2*3+2)*(2*m-3)+2*3+ 2 + (2*3+2)*(2*m-3)+2*3 + 1 + 1 +3;
  ringFixupIndices->setIndices(numIndices, SE_UNSIGNED_SHORT, NULL,
                               UT_STATIC_DRAW);
  ofs = 0;
  unsigned short *ind = (unsigned short *)ringFixupIndices->lock(AT_WRITE_ONLY);
  for(int j = 0; j < 2*m-1; j++) {
    for(int i = 0; i < 3; i++) {
      ind[ofs++] = i+j*3;
      ind[ofs++] = i+(j+1)*3;
    }
    if(j >= 2*m-2)
      break;
    if(j == m-2) {
      ind[ofs++] = 2+(j+1)*3;
      ind[ofs++] = (j+2)*3;
      j++;
      continue;
    } else {
      ind[ofs++] = 2+(j+1)*3;
      ind[ofs++] = 2+(j+1)*3;
    }
    j++;

    for(int i = 2; i >= 0; i--) {
      ind[ofs++] = i+(j+1)*3;
      ind[ofs++] = i+j*3;
    }
    ind[ofs++] = (j+2)*3;
    ind[ofs++] = (j+2)*3;
  }

  //Horizontal holes
  printf("ofs=%d\n", ofs);
  ind[ofs++] = 2+(2*m-2+1)*3;
  ind[ofs++] = 2*3*m;
  ind[ofs++] = 2*3*m;
  int starti = 2*3*m;
  for(int j = 0; j < 2*m-1; j++) {
    for(int i = 0; i < 3; i++) {
      ind[ofs++] = i+j*3+starti;
      ind[ofs++] = i+(j+1)*3+starti;
    }

    if(j==2*m-2)
      break;

    //Use 3 indices for line switching and don't use double loop
    if(j == m-2) {
      ind[ofs++] = 2+(j+1)*3+starti;
      ind[ofs++] = 2+(j+3)*3+starti;
      ind[ofs++] = 2+(j+3)*3+starti;

      ind[ofs++] = 2+(j+3)*3+starti;
      ind[ofs++] = 2+(j+3)*3+starti;
      ind[ofs++] = 2+(j+3)*3+starti;
//      ind[ofs++] = 2+(j+3)*3+starti;
      j++;
    } else {
      ind[ofs++] = 2+(j+1)*3+starti;
      ind[ofs++] = 2+(j+1)*3+starti;
    }
    j++;

    for(int i = 2; i >= 0; i--) {
      ind[ofs++] = i+(j+1)*3+starti;
      ind[ofs++] = i+j*3+starti;
    }
    if(j==2*m-2)
      break;
    ind[ofs++] = (j+2)*3+starti;
    ind[ofs++] = (j+2)*3+starti;
  }


  printf("ofs=%d est=%d\n", ofs, numIndices);
  assert(numIndices == ofs);
  ringFixupIndices->unlock();
}

/****************************************************************************/

void
ClipMap::genFinestLevel()
{
  finestLevelVertices = new GeometricBatch;
  finestLevelVertices->setPrimitiveMode(PM_TRIANGLE_STRIP);
  finestLevelVertices->setVertexFormat("vertex:float3");
  finestLevelVertices->setVertices(clipmapSize*clipmapSize*sizeof(s_vec3),
                                   NULL, UT_STATIC_DRAW);
  s_vec3 *vertices = (s_vec3 *)finestLevelVertices->lock(AT_WRITE_ONLY);
  for(int j = 0; j < clipmapSize; j++)
    for(int i = 0; i < clipmapSize; i++) {
      vertices[i+j*clipmapSize].x = i;
      vertices[i+j*clipmapSize].y = 0;
      vertices[i+j*clipmapSize].z = j;
    }
  finestLevelVertices->unlock();

  finestLevelIndices = new IndicesBatch();
  int numIndices = (2*clipmapSize+2)*(clipmapSize-2)+2*clipmapSize;
  finestLevelIndices->setIndices(numIndices, SE_UNSIGNED_SHORT, NULL,
                                 UT_STATIC_DRAW);
  int ofs = 0;
  unsigned short *ind = (unsigned short *)finestLevelIndices->lock(AT_WRITE_ONLY);
  for(int j = 0; j < clipmapSize; j++) {
    for(int i = 0; i < clipmapSize; i++) {
      ind[ofs++] = i+j*clipmapSize;
      ind[ofs++] = i+(j+1)*clipmapSize;
    }
    if(j==clipmapSize-2)
      break;
    ind[ofs++] = clipmapSize-1+(j+1)*clipmapSize;
    ind[ofs++] = clipmapSize-1+j*clipmapSize;
    j++;

    for(int i = clipmapSize-1; i >= 0; i--) {
      ind[ofs++] = i+(j+1)*clipmapSize;
      ind[ofs++] = i+j*clipmapSize;
    }
    if(j==clipmapSize-2)
      break;
    ind[ofs++] = (j+2)*clipmapSize;
    ind[ofs++] = (j+1)*clipmapSize;
  }
  assert(numIndices == ofs);
  finestLevelIndices->unlock();
}

/****************************************************************************/

void
ClipMap::render()
{
  glScalef(0.05f, 0.05f, 0.05f);
  glTranslatef(-(clipmapSize+1)/2, 0, -(clipmapSize+1)/2);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  clipmapVert->setGLMVPMatrix("mvp");

  drawBlocks(1);
  drawRingFixup(1);

  drawBlocks(2);
  drawRingFixup(2);

  // drawBlocks(3);
//  drawRingFixup(3);

  drawFinestLevel();

  Renderer::printCGError();
  Renderer::printGLError();
}
/****************************************************************************/

void
ClipMap::drawBlocks(int level)
{
  blockVertices->bind();
  blockIndices->bind();

  clipmapVert->setTextureParameter("heightmap", geomTex[level]);
  clipmapVert->bind();

  int m = (clipmapSize+1)/4;
  //Offset for mxm blocks from left-top to right-bottom
  s_vec4 offset[] = {
    {0, 0, 0, 0}, {1, 0, 0, 0}, {2, 0, 2, 0}, {3, 0, 2, 0},
    {0, 1, 0, 0}, {3, 1, 2, 0},
    {0, 2, 0, 2}, {3, 2, 2, 2},
    {0, 3, 0, 2}, {1, 3, 0, 2}, {2, 3, 2, 2}, {3, 3, 2, 2}
  };
  for(size_t i = 0; i < sizeof(offset)/sizeof(offset[0]); i++) {
    float texel = 1.0f/float(clipmapSize+1);
    glColor3ub(141, 105, 213);
    float tx = (m-1)*offset[i].x+offset[i].z;
    float ty = (m-1)*offset[i].y+offset[i].w;
    int scale = 1 << (level-1);
    clipmapVert->setParameter4f("scaleTranslate", scale, scale,
                                tx*scale-(scale-1)*(clipmapSize+1)/2,
                                ty*scale-(scale-1)*(clipmapSize+1)/2);
    tx = offset[i].x*(0.25f-texel)+offset[i].z*texel;
    ty = offset[i].y*(0.25f-texel)+offset[i].w*texel;
    clipmapVert->setParameter4f("scaleTranslateTex",
                                texel,
                                texel,
                                tx,
                                ty);


    clipmapFrag->setTextureParameter("heightmap", geomTex[level]);
    clipmapFrag->bind();
    blockVertices->drawElements(blockIndices);
    clipmapFrag->unbind();

    if(wireframe) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glEnable(GL_POLYGON_OFFSET_LINE);
      glPolygonOffset(-1, 0);
      glColor3f(level%2, 1+level%2, 0);
      glColor3f(1, 0, 0);
      blockVertices->drawElements(blockIndices);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glDisable(GL_POLYGON_OFFSET_LINE);
    }
  }
  clipmapVert->unbind();
  blockVertices->unbind();
  blockIndices->unbind();
}
/****************************************************************************/

void
ClipMap::drawRingFixup(int level)
{
  ringFixupVertices->bind();
  ringFixupIndices->bind();

  clipmapVert->setTextureParameter("heightmap", geomTex[level]);
  clipmapVert->bind();

  glColor3ub(141, 105, 213);
  int scale = 1 << (level-1);
  clipmapVert->setParameter4f("scaleTranslate", scale, scale,
                              -(scale-1)*(clipmapSize+1)/2,
                              -(scale-1)*(clipmapSize+1)/2);
  float texel = 1.0f/float(clipmapSize+1);
  //float tx =  -(scale-1)*texel*(clipmapSize+1)/2;
  //float ty =  -(scale-1)*texel*(clipmapSize+1)/2;
  float tx =  0;
  float ty =  0;
  clipmapVert->setParameter4f("scaleTranslateTex", texel*scale, texel*scale,
                              tx, ty);
  clipmapVert->setParameter4f("scaleTranslateTex", texel, texel,
                              tx, ty);

  clipmapFrag->bind();
  clipmapFrag->setTextureParameter("heightmap", geomTex[level]);

  ringFixupVertices->drawElements(ringFixupIndices);

  clipmapFrag->unbind();

  if(wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1, 0);
    glColor3f(1, 0, 0);
    ringFixupVertices->drawElements(ringFixupIndices);
//  ringFixupVertices->drawArrays();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
  }

  clipmapVert->unbind();
  ringFixupVertices->unbind();
  ringFixupIndices->unbind();
}

void
ClipMap::drawFinestLevel()
{
  finestLevelVertices->bind();
  finestLevelIndices->bind();

  clipmapVert->setTextureParameter("heightmap", geomTex[0]);
  clipmapVert->bind();

  glColor3ub(141, 105, 213);
  clipmapVert->setParameter4f("scaleTranslate", 0.5f, 0.5f, (clipmapSize+1)/4.0f,
                              (clipmapSize+1)/4.0f);
  float texel = 1.0f/float(clipmapSize+1);
  float tx =  texel*(clipmapSize+1)/4.0f;
  float ty =  texel*(clipmapSize+1)/4.0f;
  clipmapVert->setParameter4f("scaleTranslateTex", texel, texel,
                             0, 0);

  clipmapFrag->setTextureParameter("heightmap", geomTex[0]);
  clipmapFrag->bind();
  finestLevelVertices->drawElements(finestLevelIndices);
  clipmapFrag->unbind();

  if(wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1, 0);
    glColor3f(1, 0, 0);
    finestLevelVertices->drawElements(finestLevelIndices);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
  }

  clipmapVert->unbind();
  finestLevelVertices->unbind();
  finestLevelIndices->unbind();
}

ClipMap::~ClipMap()
{
}
