#ifdef WIN32
#include <windows.h>
#endif
#include <cassert>
#include <iostream>
#include <GL/glew.h>

#include "texture3d.h"
#include "renderer.h"
#include <GL/glew.h>

namespace StarEngine {

  Texture3D::Texture3D(PixelFormat pf)
    :Texture( pf )
  {
  }

  Texture3D::Texture3D(int width, int height, int depth, PixelFormat pf)
    :Texture(pf)
  {
    setData( NULL, pf, width, height, depth);
  }

  void
  Texture3D::setData(const void *data, PixelFormat pfData,
                     int width, int height, int depth)
  {
    GLint internalFormat = PixelFormatUtils::getGLFormat(pf);
    GLint format = PixelFormatUtils::getGLFormat(pfData);
    GLint type = PixelFormatUtils::getGLType(pfData);

    bind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage3DEXT( GL_TEXTURE_3D, 0, internalFormat, width, height, depth,
                     border, format, type, data );
    setMinFilter( TF_NEAREST );
    setMagFilter( TF_NEAREST );
  }

  void
  Texture3D::bind() const
  {
    glBindTexture( GL_TEXTURE_3D, glName );
  }

  void
  Texture3D::subImage(const void *data, PixelFormat pfData,
                      int width, int height, int depth,
                      int xoffset, int yoffset, int zoffset)
  {
    GLint type = PixelFormatUtils::getGLType(pfData);
    GLint format = PixelFormatUtils::getGLFormat( pfData );
    bind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage3DEXT(GL_TEXTURE_3D, 0, xoffset, yoffset, zoffset, width,
                       height, depth, format, type, data);
  }

  void
  Texture3D::setMinFilter(const TextureFilter tf)
  {
    GLint filter = getGLTextureFilter( tf );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter );
  }

  void
  Texture3D::setMagFilter(const TextureFilter tf)
  {
    GLint filter = getGLTextureFilter( tf );
    assert( tf == TF_LINEAR || tf == TF_NEAREST );
    glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filter );
  }

  GLenum
  Texture3D::getGLTarget()
  {
    return GL_TEXTURE_3D;
  }

  int
  Texture3D::getDimension()
  {
    return 3;
  }

};
