/**
 Absract base class of all textures class.
*/
#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

#include "pixelformat.h"
#include "glext.h"
#include "ressourcemanager.h"

namespace StarEngine {
  enum TextureFilter {
    TF_NEAREST,
    TF_LINEAR,
    TF_NEAREST_MIPMAP_NEAREST,
    TF_LINEAR_MIPMAP_NEAREST,
    TF_NEAREST_MIPMAP_LINEAR,
    TF_LINEAR_MIPMAP_LINEAR
  };

  enum TextureWrap {
    TW_CLAMP,
    TW_CLAMP_TO_EDGE,
    TW_REPEAT
  };

  class Texture : public Ressource {
  public:
    Texture();
    Texture( PixelFormat pf );
    virtual ~Texture();

    virtual void bind() const = 0;
    void setBorder( int b );
    /**
     Set the internal format of the texture.
     @param pf is the pixel format of the texture
    */
    void setPixelFormat( PixelFormat pf );
    virtual void setData(const void *data, PixelFormat pfData,
                         int width, int height = 0, int depth = 0) = 0;
    virtual void subImage(const void *data, PixelFormat pfData,
                          int width, int height, int depth,
                          int xoffset, int yoffset = 0, int zoffset = 0) = 0;

    virtual void setMinFilter(const TextureFilter tf) = 0;
    virtual void setMagFilter(const TextureFilter tf) = 0;
    virtual void setWrapS(const TextureWrap tf);
    virtual void setWrapT(const TextureWrap tf);
    virtual void setWrapR(const TextureWrap tf);
    virtual int getDimension() = 0;
    virtual GLenum getGLTarget() = 0;
    virtual GLuint getGLName() { return glName; }

  protected:
    GLuint glName;
    PixelFormat pf;
    int border;

    GLint getGLTextureFilter(TextureFilter tf);
    GLenum getGLTextureWrap(TextureWrap tw);
  };
};

#endif
