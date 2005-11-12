#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "texture.h"

namespace StarEngine {
  class Texture2D : public Texture {
  public:
    Texture2D(PixelFormat pf);
    Texture2D(int width, int height, PixelFormat pf);
    void bind() const;
    void setData(const void *data, int level, PixelFormat pfData,
                 int width, int height, int depth = 1);
    void subImage(const void *data, PixelFormat pfData,
                  int width, int height, int depth,
                  int xoffset, int yoffset, int zoffset = 0);
    void setMinFilter(const TextureFilter tf);
    void setMagFilter(const TextureFilter tf);
    GLenum getGLTarget();
    int getDimension();
  };
}

#endif

