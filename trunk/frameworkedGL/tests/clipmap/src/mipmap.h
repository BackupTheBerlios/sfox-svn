#ifndef MIPMAP_H
#define MIPMAP_H

#include "starengine.h"
#include <vector>

class Mipmap {
public:
  void getTexture(StarEngine::Texture2D *texture, int xoffs, int yoffs,
                  int width, int height);

  void buildMipmap(const char *filename, int levels);

  StarEngine::Image *getLevel(int n);

private:
  std::vector<StarEngine::Image *> levels;
  StarEngine::Image *halfScale(StarEngine::Image *img);
};

#endif
