#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>
#include <cassert>

#include "object3d.h"
#include "vertexbuffer.h"
#include "mesh.h"
#include "material.h"

namespace StarEngine {
  Object3d::Object3d(Mesh *m)
    : mat(0)
  {
    Vertex *vec = m->getVertex(&numVertices);
    vertices = new VertexBuffer(GL_ARRAY_BUFFER_ARB, numVertices*sizeof(Vertex),
                                GL_STATIC_DRAW_ARB, vec);
    delete[] vec;

    unsigned int *in = m->getFaces();
    numFaces = m->getNumFaces();
    indices = new VertexBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB,
                               numFaces*sizeof(int)*3,
                               GL_STATIC_DRAW_ARB, in);
    delete[] in;
  }

  Object3d::~Object3d()
  {
    delete vertices;
    delete indices;
    delete mat;
    vertices = indices = 0;
    mat = 0;
  }

  void
  Object3d::bind()
  {
    assert(vertices&&indices);
    vertices->bind();
    indices->bind();
    vertices->texCoordPointer(2, sizeof(Vertex), OFFSETTCOORD);
    vertices->vertexPointer(3, sizeof(Vertex), 0);
    vertices->normalPointer(sizeof(Vertex), OFFSETNORM);
  }

  void
  Object3d::render()
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    indices->drawElements(GL_TRIANGLES);
  }
}
