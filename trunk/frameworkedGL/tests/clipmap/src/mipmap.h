#ifndef MIPMAP_H
#define MIPMAP_H

#include "starengine.h"

class Mipmap {
public:
  void buildMipmap(const char *filename, int levels);

  StarEngine::Image *getLevel(int n);

private:
  StarEngine::Image **levels;
  StarEngine::Image *halfScale(StarEngine::Image *img);
};

#endif
