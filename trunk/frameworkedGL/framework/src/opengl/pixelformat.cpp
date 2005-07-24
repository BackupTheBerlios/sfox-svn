#ifdef _WIN32
#include <windows.h>
#endif
#include <cassert>
#include <GL/gl.h>
#include <GL/glext.h>

#include "pixelformat.h"

namespace StarEngine {
  GLint
  PixelFormatUtils::getGLFormat(PixelFormat pf)
  {
    switch( pf ) {
    case PF_ALPHA8:
      return GL_ALPHA;
    case PF_LUMINANCE:
      return GL_LUMINANCE;
    case PF_RGB:
      return GL_RGB;
    case PF_RGBA:
      return GL_RGBA;
    case PF_DEPTH16:
      return GL_DEPTH_COMPONENT16_ARB;
    case PF_DEPTH24:
      return GL_DEPTH_COMPONENT24_ARB;
    case PF_DEPTH32:
      return GL_DEPTH_COMPONENT32_ARB;
    default:
      assert( 0 );
      break;
    }
    return -1;
  }

  int
  PixelFormatUtils::getBytesPerPixel(PixelFormat pf)
  {
    switch( pf ) {
    case PF_ALPHA8:
      return 1;
    case PF_LUMINANCE:
      return 1;
    case PF_RGB:
      return 3;
    case PF_RGBA:
      return 4;
    default:
      assert( 0 );
      break;
    }
    return -1;
  }

  GLint
  PixelFormatUtils::getGLType(PixelFormat pf)
  {
    switch( pf ) {
    case PF_RGB:
    case PF_RGBA:
    case PF_ALPHA8:
      return GL_UNSIGNED_BYTE;
    case PF_DEPTH16:
    case PF_DEPTH24:
    case PF_DEPTH32:
      return GL_DEPTH_COMPONENT;
    default:
      assert( 0 );
      break;
    }
    return -1;
  }
};
