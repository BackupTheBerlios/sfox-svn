#include <cassert>

#include "textureunit.h"
#include <GL/glew.h>

namespace StarEngine {
  void
  TextureUnits::activeUnit( GLenum unit )
  {
    glActiveTextureARB(GL_TEXTURE0_ARB+unit);
  }

  void
  TextureUnits::setEnvMode(TextureEnvMode mode)
  {
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, getGLEnvMode( mode ) );
  }

  GLint
  TextureUnits::getGLEnvMode( TextureEnvMode mode )
  {
    switch( mode ) {
    case   TEM_MODULATE:
      return  GL_MODULATE;
    case     TEM_DECAL:
      return  GL_DECAL;
    case     TEM_BLEND:
      return  GL_BLEND;
    case     TEM_REPLACE:
      return  GL_REPLACE;
    default:
      assert( 0 );
      break;
    }
    return -1;
  }
};
