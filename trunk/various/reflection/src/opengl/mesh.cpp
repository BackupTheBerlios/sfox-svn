#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "exception.h"
#include "mesh.h"

using namespace std;

Mesh::Mesh(const char *filename)
{
  fs.open(filename, fstream::in|fstream::binary);
}

unsigned int
Mesh::findChunk(const char *chunkName)
{
  unsigned int len = 0, notFound;
  char tmp[256];
  fs.clear();
  fs.seekg(0, ios::beg);
  do {
    fs.seekg(len, ios::cur);
    fs.getline(tmp,256,'\0');
    fs.read((char *)&len, sizeof(int));
  } while((notFound=strcmp(tmp, chunkName))!=0 &&!fs.eof());

  if(notFound) {
    std::string mess("Can't find chunk: ");
    throw new Exception(mess.append(chunkName));
  }
  
  return len;
}

Vec3f *
Mesh::getVertices()
{
  unsigned int size = findChunk("VERTICES");
  Vec3f *ret = new Vec3f[size/sizeof(Vec3f)];

  fs.read((char *)ret, size);

  return ret;
}

unsigned int *
Mesh::getFaces()
{
  unsigned int size = findChunk("INDICES");
  unsigned int *ret = new unsigned int[size/sizeof(unsigned int)];
  
  fs.read((char *)ret, size);  
  
  return ret;
}

Vec3f *
Mesh::getNormals()
{
  unsigned int size = findChunk("NORMALS");
  Vec3f *ret = new Vec3f[size/sizeof(Vec3f)];

  fs.read((char *)ret, size);  
  
  return ret;
}

Vec2f *
Mesh::getUV()
{
  try {
    unsigned int size = findChunk("UV");
    Vec2f *ret = new Vec2f[size/sizeof(Vec2f)];

    fs.read((char *)ret, size);
    return ret;
  } catch(Exception *e) {
    //UV chunk is optionnal
    return 0;
  }  
}

unsigned int
Mesh::getNumNormals()
{
  return findChunk("NORMALS")/sizeof(Vec3f);
}

unsigned int
Mesh::getNumUV()
{
  return findChunk("UV")/(2*sizeof(float));
}

unsigned int
Mesh::getNumFaces()
{
  return findChunk("INDICES")/(sizeof(unsigned int)*3);
}

unsigned int
Mesh::getNumVertices()
{
  return findChunk("VERTICES")/sizeof(Vec3f);
}

Vertex *
Mesh::getVertex(unsigned int *size)
{
  int n = getNumVertices();
  *size = n;
  Vec3f *vec = getVertices();
  Vertex *vertex = new Vertex[n];
  for(int i = 0; i < n; i++) {
    vertex[i].x = vec[i].x;
    vertex[i].y = vec[i].y;
    vertex[i].z = vec[i].z;
  }
  delete[] vec;

  vec = getNormals();
  for(int i = 0; i < n; i++) {
    vertex[i].nx = vec[i].x;
    vertex[i].ny = vec[i].y;
    vertex[i].nz = vec[i].z;
  }
  delete[] vec;

  Vec2f *vec2 = getUV();
  if(vec2) {
    for(int i = 0; i < n; i++) {
      vertex[i].u = vec2[i].x;
      vertex[i].v = vec2[i].y;
      //std::cerr << "(" << vertex[i].u << ", " << vertex[i].v << ")" << std::endl;
    }
    delete[] vec2;
  }
   
  return vertex;
}

Mesh::~Mesh()
{
  fs.close();
}
