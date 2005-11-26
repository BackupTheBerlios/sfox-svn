#ifndef MIPMAP_H
#define MIPMAP_H

#include "starengine.h"
#include <vector>

class Mipmap {
public:
  void getTextures(std::vector<StarEngine::Texture2D *> &textures,
                   StarEngine::PixelFormat pf, int xoffs, int yoffs,
                   int width, int height);

  void buildMipmap(const char *filename, int levels);
  void buildMipmapGPU(const char *filename, int numLevels);

  StarEngine::Image *getLevel(int n);

private:
  std::vector<StarEngine::Image *> levels;
  StarEngine::Image *halfScale(StarEngine::Image *img);

  StarEngine::Image * halfScaleGPU(StarEngine::Image *img);
};

#endif
