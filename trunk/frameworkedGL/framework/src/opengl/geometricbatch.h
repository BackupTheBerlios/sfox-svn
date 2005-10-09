#ifndef GEOMETRICBATCH_H
#define GEOMETRICBATCH_H

#include <vector>
#include <string>

namespace StarEngine {
  class GeometricBatch {
  public:
    GeometricBatch();
    void setVertexFormat(const std::string &format);
  private:
    int stride;
    std::vector<int> attribsOffset;
    std::vector<std::string> attribsNames;

    typedef std::pair<std::string, std::string> FormatPair;
    std::vector<FormatPair> extractFormat(const std::string &format);
    void getTypeDescription(const std::string &type, int &numComponents,
                            int &typeIndice);

  };
}

#endif
