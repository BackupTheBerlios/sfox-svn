#ifndef MESH_H
#define MESH_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <fstream>
#include <string>
#include <vector>

#include "math/vector2.h"
#include "math/vector3.h"
#include "vertex.h"

namespace StarEngine {
  class Mesh {
  private:
    std::ifstream fs;
    std::string name;

    unsigned int findChunk(const char *chunkName);

  public:
    Mesh(const char *filename);
    ~Mesh();

    Vec3f *getVertices();
    Vec3f *getNormals();
    Vec2f *getUV();
    unsigned int *getFaces();

    unsigned int getNumNormals();
    unsigned int getNumFaces();
    unsigned int getNumVertices();
    unsigned int getNumUV();

    Vertex * getVertex(unsigned int *size);
  };
}
#endif
