#ifndef GEOMETRICBATCH_H
#define GEOMETRICBATCH_H

#include <vector>
#include <string>

#include "opengl/startypes.h"

namespace StarEngine {
  class IndicesBatch;

  class GeometricBatch {
  public:
    GeometricBatch();
    ~GeometricBatch();

    void setVertexFormat(const std::string &format);
    void setPrimitiveMode(PrimitiveMode pt);
    /*Size is in octet*/
    void setVertices(int size, void *data, UsageType usage);
    void drawArrays(int first = 0, int count = -1);
    void drawElements(IndicesBatch *indices, int count = -1);
    void *lock(AccessType access);
    void unlock();

    void bind();
    void unbind();

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
    void enablePointers();
    void setPointers();

    int m_stride;
    struct PointerParam {
      int numComponents;
      int descIndex;
      int pos;
    };
    std::vector<PointerParam> m_pointerParam;
  };

/*****************************************************************************/
  class IndicesBatch {
  public:
    IndicesBatch();
    ~IndicesBatch();
    void setIndices(int numIndices, Type type, void *data, UsageType usage);
    Type getType() { return type; }

    void *lock(AccessType access);
    void unlock();

    void bind();
    void unbind();

    int getNumIndices() { return numIndices; }

  private:
    Type type;
    GLuint indicesBufferId;
    unsigned int numIndices;

    GLuint getBufferObject() { return indicesBufferId; }
  };

}

#endif
