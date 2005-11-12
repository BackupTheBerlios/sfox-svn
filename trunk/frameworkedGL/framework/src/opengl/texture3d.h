#ifndef TEXTURE3D_H
#define TEXTURE3D_H

#include "texture.h"

namespace StarEngine {
  class Texture3D : public Texture {
  public:
    Texture3D(PixelFormat pf);
    Texture3D(int width, int height, int depth, PixelFormat pf);
    void bind() const;
    void setData(const void *data, int level, PixelFormat pfData,
                 int width, int height, int depth);
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
