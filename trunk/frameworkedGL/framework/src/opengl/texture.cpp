#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <string>
#include <cassert>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <SDL.h>

#include "texture.h"
#include "framework/exception.h"

namespace StarEngine {
  Texture::Texture()
    : border( 0 )
  {
    glGenTextures( 1,  &glName );
  }


  Texture::Texture( PixelFormat pf )
    : pf( pf ), border( 0 )
  {
    glGenTextures( 1,  &glName );
  }

  Texture::~Texture()
  {
    glDeleteTextures( 1, &glName );
  }

  void
  Texture::setBorder( int b )
  {
    assert(b==1||b==0);
    border = b;
  }

  void
  Texture::setPixelFormat( PixelFormat pf )
  {
    this->pf = pf;
  }

  GLint
  Texture::getGLTextureFilter(TextureFilter tf)
  {
    switch( tf ) {
    case TF_NEAREST:
      return  GL_NEAREST;
    case TF_LINEAR:
      return  GL_LINEAR;
    case TF_NEAREST_MIPMAP_NEAREST:
      return  GL_NEAREST_MIPMAP_NEAREST;
    case TF_LINEAR_MIPMAP_NEAREST:
      return  GL_LINEAR_MIPMAP_NEAREST;
    case TF_NEAREST_MIPMAP_LINEAR:
      return  GL_NEAREST_MIPMAP_LINEAR;
    case TF_LINEAR_MIPMAP_LINEAR:
      return  GL_LINEAR_MIPMAP_LINEAR;
    default:
      assert( 0 );
      break;
    }
    return -1;
  }
}
