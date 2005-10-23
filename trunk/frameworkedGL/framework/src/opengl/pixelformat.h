#ifndef FRAMEWORK_PIXELFORMAT_H
#define FRAMEWORK_PIXELFORMAT_H

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>

namespace StarEngine {
  enum PixelFormat {
    PF_DEFAULT = -1,
    PF_ALPHA8,
    PF_LUMINANCE,
    PF_RGB,
    PF_RGBA,
    PF_DEPTH,
    PF_DEPTH16,
    PF_DEPTH24,
    PF_DEPTH32,
    PF_RGB16F,
    PF_RGBA16F,
    PF_RGB32F,
    PF_RGBA32F
  };

  enum DataType {
    DT_DEFAULT = -1,
    DT_UNSIGNED_BYTE,
    DT_FLOAT
  };

  class PixelFormatUtils {
  public:
    static GLint getGLInternalFormat( PixelFormat pf );
    static GLint getGLFormat( PixelFormat pf );
    static GLint getGLType( PixelFormat pf );
    static int getBytesPerPixel(PixelFormat pf);
  };
};

#endif
