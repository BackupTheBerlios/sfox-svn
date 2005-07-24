#ifndef OBJECT3D_H
#define OBJECT3D_H

namespace StarEngine {
  class VertexBuffer;
  class Mesh;
  class Material;

  class Object3d {
  private:
    Material *mat; //Destructor deletes everything
    VertexBuffer *vertices, *indices;
    unsigned int numVertices, numFaces;

  public:
    Object3d(Mesh *m);
    ~Object3d();

    void bind();
    void render();
    void setMaterial(Material *m) { mat = m; }
    Material *getMaterial() { return mat; }

    VertexBuffer *getVertexBuffer() { return vertices; }
    VertexBuffer *getIndexBuffer() { return indices; }
    unsigned int getNumVertices() { return numVertices; }
    unsigned int getNumFaces() { return numFaces; }
  };
}
#endif
