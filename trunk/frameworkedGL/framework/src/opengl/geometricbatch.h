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

  class GeometricBatch {
  public:
    GeometricBatch();
    ~GeometricBatch();

    void setVertexFormat(const std::string &format);
    void setPrimitiveMode(PrimitiveMode pt);
    void setIndices(int size, void *data, GLenum usage);
    void setVertices(int size, void *data, GLenum usage);
    void drawArrays(int first = 0, int count = -1);

  private:
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
  };
}

#endif
