#include <cassert>
#include <iostream>

#include "texture2d.h"
#include "renderer.h"

namespace StarEngine {

  Texture2D::Texture2D(PixelFormat pf)
    :Texture( pf )
  {
  }

  Texture2D::Texture2D(int width, int height, PixelFormat pf)
    :Texture(pf)
  {
    setData(NULL, 0, pf, width, height);
  }

  void
  Texture2D::setData(const void *data, int level, PixelFormat pfData,
                     int width, int height, int depth)
  {
    GLint internalFormat = PixelFormatUtils::getGLInternalFormat(pf);
    GLint format = PixelFormatUtils::getGLFormat(pfData);
    GLint type = PixelFormatUtils::getGLType(pfData);

    bind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D( GL_TEXTURE_2D, level, internalFormat, width, height, border,
                  format, type, data );
  }

  void
  Texture2D::bind() const
  {
    glBindTexture( GL_TEXTURE_2D, glName );
  }

  void
  Texture2D::subImage(const void *data, PixelFormat pfData,
                      int width, int height, int depth,
                      int xoffset, int yoffset, int zoffset)
  {
    GLint type = PixelFormatUtils::getGLType(pfData);
    GLint format = PixelFormatUtils::getGLFormat( pfData );
    bind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height,
                    format, type, data);
  }

  void
  Texture2D::setMinFilter(const TextureFilter tf)
  {
    GLint filter = getGLTextureFilter( tf );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter );
  }

  void
  Texture2D::setMagFilter(const TextureFilter tf)
  {
    GLint filter = getGLTextureFilter( tf );
    assert( tf == TF_LINEAR || tf == TF_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter );
  }

  GLenum
  Texture2D::getGLTarget()
  {
    return GL_TEXTURE_2D;
  }

  int
  Texture2D::getDimension()
  {
    return 2;
  }

};
