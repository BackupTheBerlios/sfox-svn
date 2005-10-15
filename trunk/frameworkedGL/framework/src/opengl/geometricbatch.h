#ifndef GEOMETRICBATCH_H
#define GEOMETRICBATCH_H

#include <vector>
#include <string>

namespace StarEngine {
  enum PrimitiveMode {
    PM_POINTS,
    PM_LINE_STRIP,
    PM_LINE_LOOP,
    PM_LINES,
    PM_TRIANGLE_STRIP,
    PM_TRIANGLE_FAN,
    PM_TRIANGLES,
    PM_QUAD_STRIP,
    PM_QUADS,
    PM_POLYGON,
    PM_UNKNOWN
  };

  enum Type {
    SE_UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    SE_UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    SE_UNSIGNED_INT = GL_UNSIGNED_INT
  };

  int getSizeOfType(Type type);

  class IndicesBatch;

  class GeometricBatch {
  public:
    GeometricBatch();
    ~GeometricBatch();

    void setVertexFormat(const std::string &format);
    void setPrimitiveMode(PrimitiveMode pt);
    void setIndices(IndicesBatch *indices);
    void setVertices(int size, void *data, GLenum usage);
    void drawArrays(int first = 0, int count = -1);
    void drawElements(int count = -1);

  private:
    IndicesBatch *indices;
    GLuint verticesBufferId;
    GLuint indicesBufferId;
    PrimitiveMode primitiveMode;
    unsigned int numVertices;
    unsigned int numIndices;
    unsigned int enabledPointers;
    std::string vertexFormat;

    typedef std::pair<std::string, std::string> FormatPair;
    std::vector<FormatPair> extractFormat(const std::string &format);
    void getTypeDescription(const std::string &type, int &numComponents,
                            int &typeIndice);
    int getAttribDefaultPos(const std::string &name);
    int computeStride(std::vector<FormatPair> vf);
    GLenum getGLPrimitiveMode(PrimitiveMode pt);
    void enablePointers();
  };

  class IndicesBatch {
  public:
    IndicesBatch();
    ~IndicesBatch();
    void setIndices(int numIndices, Type type, GLenum usage, void *data);
    Type getType() { return type; }

  private:
    Type type;
    GLuint indicesBufferId;
    unsigned int numIndices;

    GLuint getBufferObject() { return indicesBufferId; }
    friend void GeometricBatch::drawElements(int count);
  };

}

#endif
