#ifndef TEXTURE_UNIT_H
#define TEXTURE_UNIT_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

namespace StarEngine {
  enum TextureEnvMode {
    TEM_MODULATE,
    TEM_DECAL,
    TEM_BLEND,
    TEM_REPLACE
  };

  class TextureUnits {
  public:
    static void activeUnit( GLenum unit );
    static void setEnvMode( TextureEnvMode mode );

  private:
    static GLint getGLEnvMode( TextureEnvMode mode );
  };
}

#endif

