#ifdef WIN32
#include <windows.h>
#endif
#include <cassert>
#include <GL/glew.h>


#include "pixelformat.h"

namespace StarEngine {
  GLint
  PixelFormatUtils::getGLFormat(PixelFormat pf)
  {
    switch( pf ) {
    case PF_RED:
      return GL_RED;
    case PF_ALPHA8:
      return GL_ALPHA;
    case PF_LUMINANCE:
    case PF_LUMINANCE32F:
      return GL_LUMINANCE;
    case PF_INTENSITY:
    case PF_INTENSITY32F:
      return GL_INTENSITY;
    case PF_RGB16F:
    case PF_RGB32F:
    case PF_RGB:
      return GL_RGB;
    case PF_RGBA16F:
    case PF_RGBA32F:
    case PF_RGBA:
      return GL_RGBA;
    case PF_DEPTH:
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

  GLint
  PixelFormatUtils::getGLInternalFormat(PixelFormat pf)
  {
    switch( pf ) {
    case PF_DEPTH16:
      return GL_DEPTH_COMPONENT16_ARB;
    case PF_DEPTH24:
      return GL_DEPTH_COMPONENT24_ARB;
    case PF_DEPTH32:
      return GL_DEPTH_COMPONENT32_ARB;
    case PF_RGB16F:
      return GL_RGB16F_ARB;
    case PF_RGBA16F:
      return GL_RGBA16F_ARB;
    case PF_RGB32F:
      return GL_RGB32F_ARB;
    case PF_RGBA32F:
      return GL_RGBA32F_ARB;
    case PF_INTENSITY32F:
      return GL_INTENSITY32F_ARB;
    case PF_LUMINANCE32F:
      return GL_LUMINANCE32F_ARB;
    default:
      return getGLFormat( pf );
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
    case PF_RED:
    case PF_LUMINANCE:
    case PF_INTENSITY:
    case PF_INTENSITY32F:
    case PF_LUMINANCE32F:
      return 1;
    case PF_RGB:
    case PF_RGB32F:
    case PF_RGB16F:
      return 3;
    case PF_RGBA16F:
    case PF_RGBA32F:
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
    case PF_RED:
    case PF_RGB:
    case PF_RGBA:
    case PF_LUMINANCE:
    case PF_INTENSITY:
    case PF_ALPHA8:
      return GL_UNSIGNED_BYTE;
    case PF_DEPTH:
    case PF_DEPTH16:
    case PF_DEPTH24:
    case PF_DEPTH32:
      return GL_UNSIGNED_BYTE;
    case PF_RGB16F:
    case PF_RGBA16F:
    case PF_RGB32F:
    case PF_RGBA32F:
    case PF_INTENSITY32F:
    case PF_LUMINANCE32F:
      return GL_FLOAT;
    default:
      assert( 0 );
      break;
    }
    return -1;
  }
};
