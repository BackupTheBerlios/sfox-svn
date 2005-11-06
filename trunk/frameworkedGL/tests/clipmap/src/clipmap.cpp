#include "clipmap.h"

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
  : clipmapSize(n)
{
  genBlocks();
  genRingFixUp();

  //Load float terrain texture
  Image *img = new Image;
  img->load(DATAPATH"/media/clipmap/terrain/terrain1.png");
  ImageLoader::ImageData *imgData = img->getImageData();
  Texture *tex = g_TextureManager.create("terrain", PF_RGBA32F, 256, 256);
  tex->setData( imgData->data, imgData->pixelFormat, imgData->width,
                imgData->height );

  clipmapVert = new ShaderCG();
  clipmapVert->loadSourceFromFile("shaders/clipmap_vert.cg",
                                  ShaderCG::VERTEX);

  clipmapFrag = new ShaderCG();
  clipmapFrag->loadSourceFromFile("shaders/clipmap_frag.cg",
                                  ShaderCG::FRAGMENT);
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
// void
// ClipMap::genRingFixUp()
// {
//   int m = (clipmapSize+1)/4;

//   ringFixupVertices = new GeometricBatch;
//   ringFixupVertices->setPrimitiveMode(PM_TRIANGLE_STRIP);
// //  ringFixupVertices->setPrimitiveMode(PM_POINTS);
//   ringFixupVertices->setVertexFormat("vertex:float3");
//   ringFixupVertices->setVertices(m*3*4*sizeof(s_vec3), NULL, UT_STATIC_DRAW);
//   s_vec3 *vertices = (s_vec3 *)ringFixupVertices->lock(AT_READ_WRITE);
//   float startx = 2*(m-1);
//   for(int k = 0; k < 2; k++) {
//     for(int j = 0; j < m; j++)
//       for(int i = 0; i < 3; i++) {
//         int ofs = i+j*3+k*3*m;
//         vertices[ofs].x = startx+i;
//         vertices[ofs].y = 0;
//         vertices[ofs].z = j+k*(3*(m-1)+2);
//       }
//   }
//   int ofs = 2*3*m;
//   for(int i = 0; i < 2*3*m; i++) {
//     vertices[ofs].x = vertices[i].z;
//     vertices[ofs].y = 0;
//     vertices[ofs++].z = vertices[i].x;
//   }
//   ringFixupVertices->unlock();

//   ringFixupIndices = new IndicesBatch();
//   int numIndices = ((2*3+2)*(2*m-3)+2*3);
//   ringFixupIndices->setIndices(numIndices, SE_UNSIGNED_SHORT, NULL,
//                                UT_STATIC_DRAW);
//   ofs = 0;
//   unsigned short *ind = (unsigned short *)ringFixupIndices->lock(AT_WRITE_ONLY);
//   for(int j = 0; j < 2*m-1; j++) {
//     for(int i = 0; i < 3; i++) {
//       ind[ofs++] = i+j*3;
//       ind[ofs++] = i+(j+1)*3;
//     }
//     if(j >= 2*m-2)
//       break;
//     if(j == m-2) {
//       ind[ofs++] = 2+(j+1)*3;
//       ind[ofs++] = (j+2)*3;
//       j++;
//       continue;
//     } else {
//       ind[ofs++] = 2+(j+1)*3;
//       ind[ofs++] = 2+(j+1)*3;
//     }
//     j++;

//     for(int i = 2; i >= 0; i--) {
//       ind[ofs++] = i+(j+1)*3;
//       ind[ofs++] = i+j*3;
//     }
//     ind[ofs++] = (j+2)*3;
//     ind[ofs++] = (j+2)*3;
//   }
//     printf("ofs=%d est=%d\n", ofs, numIndices);
//   assert(numIndices == ofs);
//   ringFixupIndices->unlock();
// }
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
ClipMap::render()
{
  glScalef(0.05,0.05,0.05);
  glTranslatef(-(clipmapSize+1)/2, 0, -(clipmapSize+1)/2);
  glPolygonMode(GL_FRONT, GL_FILL);
  Texture * texTerrain = g_TextureManager.getByName("terrain");
  texTerrain->bind();
  texTerrain->setMinFilter( TF_NEAREST );
  texTerrain->setMagFilter( TF_NEAREST );
  texTerrain->setWrapS( TW_CLAMP_TO_EDGE );
  texTerrain->setWrapT( TW_CLAMP_TO_EDGE );

  clipmapVert->bind();
  clipmapVert->setGLMVPMatrix("mvp");
  clipmapVert->setTextureParameter("heightmap", texTerrain);
  clipmapVert->enableTextureParameter("heightmap");

  drawBlocks();
  drawRingFixup();
  clipmapVert->disableTextureParameter("heightmap");
  clipmapVert->unbind();

//  drawRingFixup();

  Renderer::printCGError();
  Renderer::printGLError();
}
/****************************************************************************/

void
ClipMap::drawBlocks()
{
  blockVertices->bind();
  blockIndices->bind();

  int m = (clipmapSize+1)/4;
  //Offset for mxm blocks from left-top to right-bottom
  s_vec4 offset[] = {
    {0, 0, 0, 0}, {1, 0, 0, 0}, {2, 0, 2, 0}, {3, 0, 2, 0},
    {0, 1, 0, 0}, {3, 1, 2, 0},
    {0, 2, 0, 2}, {3, 2, 2, 2},
    {0, 3, 0, 2}, {1, 3, 0, 2}, {2, 3, 2, 2}, {3, 3, 2, 2}
  };
  for(size_t i = 0; i < sizeof(offset)/sizeof(offset[0]); i++) {
    glColor3ub(141, 105, 213);
    float tx = (m-1)*offset[i].x+offset[i].z;
    float ty = (m-1)*offset[i].y+offset[i].w;
    clipmapVert->setParameter4f("scaleTranslate", 1, 1, tx, ty);
//     tx =  offset[i].x*(0.25-1./256.)+offset[i].z*2./256.;
//     ty =  offset[i].y*(0.25-1./256.)+offset[i].w*2./256.;
    tx =  offset[i].x*(0.25-1./256.)+offset[i].z*1./256.;
    ty =  offset[i].y*(0.25-1./256.)+offset[i].w*1./256.;
    clipmapVert->setParameter4f("scaleTranslateTex", 1./256, 1./256,
                                tx, ty);

    clipmapFrag->bind();
    Texture * texTerrain = g_TextureManager.getByName("terrain");
    clipmapFrag->setTextureParameter("heightmap", texTerrain);
    clipmapFrag->enableTextureParameter("heightmap");

    blockVertices->drawElements(blockIndices);

    clipmapFrag->disableTextureParameter("heightmap");
    clipmapFrag->unbind();

 #if 0
    glPolygonMode(GL_FRONT, GL_LINE);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1, 0);
    glColor3f(1, 0, 0);
    blockVertices->drawElements(blockIndices);
    glPolygonMode(GL_FRONT, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
#endif
  }
  blockVertices->unbind();
  blockIndices->unbind();
}
/****************************************************************************/

void
ClipMap::drawRingFixup()
{
  int m = (clipmapSize+1)/4;
  ringFixupVertices->bind();
  ringFixupIndices->bind();

  glColor3ub(141, 105, 213);
//  clipmapVert->setParameter4f("scaleTranslate", 1, 1, 0, -2*(m-1)+1);
  clipmapVert->setParameter4f("scaleTranslate", 1, 1, 0, 0);
  float tx =  0./256.;
  float ty =  0;
  clipmapVert->setParameter4f("scaleTranslateTex", 1./256, 1./256,
                              tx, ty);

  clipmapFrag->bind();
  Texture * texTerrain = g_TextureManager.getByName("terrain");
  clipmapFrag->setTextureParameter("heightmap", texTerrain);
  clipmapFrag->enableTextureParameter("heightmap");

//  ringFixupVertices->drawElements(ringFixupIndices, 1012);
//  ringFixupVertices->drawElements(ringFixupIndices, 800);
  ringFixupVertices->drawElements(ringFixupIndices);

  clipmapFrag->disableTextureParameter("heightmap");
  clipmapFrag->unbind();

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glDisable(GL_CULL_FACE);
//   //glEnable(GL_POLYGON_OFFSET_LINE);
//   glPolygonOffset(-1, 0);
//   glColor3f(1, 0, 0);
//   ringFixupVertices->drawElements(ringFixupIndices);
// //  ringFixupVertices->drawArrays();
//   glPolygonMode(GL_FRONT, GL_FILL);
//   glDisable(GL_POLYGON_OFFSET_LINE);

  ringFixupVertices->unbind();
  ringFixupIndices->unbind();
}

ClipMap::~ClipMap()
{
}
