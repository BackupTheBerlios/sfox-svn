#ifndef GEOMETRICBATCH_H
#define GEOMETRICBATCH_H

#include <vector>
#include <string>

namespace StarEngine {
  class GeometricBatch {
  public:
    GeometricBatch();
    void setVertexFormat(std::string format);
  private:
    int stride;
    std::vector<int> attribsOffset;
    std::vector<std::string> attribsNames;
  };
}

#endif
