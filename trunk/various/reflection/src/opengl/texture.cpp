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
#include <SDL_image.h>

#include "texture.h"
#include "exception.h"

Texture::Texture(int width, int height, GLuint target, int flag)
  : target(target)
{
  extractParams(flag);
  genTexture();
  glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, NULL);
  texParameter(GL_TEXTURE_MAG_FILTER, magFilter);
  texParameter(GL_TEXTURE_MIN_FILTER, minFilter);
}


Texture::Texture(const char *filename, GLuint target, int flag)
  : name(filename), target(target)
{
  flag |= UNSIGNED_BYTE|RGB; //RGB avoid an exception in extractParams
  extractParams(flag);
  genTexture();

  if(target == TEXTURE_CUBE_MAP)
    loadCubemap(flag);
  else if(target == TEXTURE_2D)
    loadTexture2D(name, target, flag);

  texParameter(GL_TEXTURE_MAG_FILTER, magFilter);
  texParameter(GL_TEXTURE_MIN_FILTER, minFilter);
}

Texture::~Texture()
{
  glDeleteTextures(1, &texName);
}

void
Texture::extractParams(int flag)
{
  minFilter = magFilter = GL_NEAREST;
  
  if(flag&ALPHA) format = GL_ALPHA;
  else if(flag&RGB) format = GL_RGB;
  else if(flag&RGBA) format = GL_RGBA;
  else if(flag&LUMINANCE) format = GL_LUMINANCE;
  else throw new Exception("genTexture: Unknown texture format");
  internalFormat = format;

  if(flag&UNSIGNED_BYTE) type = GL_UNSIGNED_BYTE;
  else if(flag&FLOAT) type = GL_FLOAT;
  else throw new Exception("genTexture: Unknown type");

  if(flag&NEAREST) magFilter = minFilter = GL_NEAREST;
  else if(flag&LINEAR) magFilter = minFilter = GL_LINEAR;
  else if(flag&NEAREST_MIPMAP_NEAREST) {
    minFilter = GL_NEAREST_MIPMAP_NEAREST;
    magFilter = GL_NEAREST;
  } else if(flag&LINEAR_MIPMAP_NEAREST) {
    minFilter = GL_LINEAR_MIPMAP_NEAREST;
    magFilter = GL_NEAREST;
  } else if(flag&LINEAR_MIPMAP_LINEAR) {
    minFilter = GL_LINEAR_MIPMAP_LINEAR;
    magFilter = GL_LINEAR;
  }
}

void
Texture::genTexture()
{
  m.toIdentity();

  glGenTextures(1, &texName);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(target, texName);
}

/*****************************************************************************/
/* Wrapped opengl functions                                                  */
/*****************************************************************************/
void
Texture::bind()
{
  glMatrixMode(GL_TEXTURE);
  glLoadMatrixf(m[0]);
  glMatrixMode(GL_MODELVIEW);

  glBindTexture(target, texName);
}

void
Texture::subImage(const void *pixels, int xoffset, int yoffset, int width,
                  int height, int flag)
{
  int subFormat;
  if(flag&ALPHA) subFormat = GL_ALPHA;
  else if(flag&RGB) subFormat = GL_RGB;
  else if(flag&RGBA) subFormat = GL_RGBA;
  else if(flag&LUMINANCE) subFormat = GL_LUMINANCE;
  else throw new Exception("copyData: Unknown texture format");

  glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, subFormat,
                  GL_UNSIGNED_BYTE, pixels);  
}

/*****************************************************************************/
/* Env and parameters related methods                                        */
/*****************************************************************************/
void
Texture::texParameter(GLenum pname, GLuint param)
{
  glTexParameteri(target, pname, param);
}

void
Texture::setTexEnv(GLenum pname, GLint param)
{
  glTexEnvi(GL_TEXTURE_ENV, pname, param);
}

void
Texture::setTexEnv(GLenum pname, GLfloat param)
{
  glTexEnvf(GL_TEXTURE_ENV, pname, param);
}

void
Texture::setTexGen(GLenum coord, GLint mode)
{
  glTexGeni(coord, GL_TEXTURE_GEN_MODE, mode);
}

/*****************************************************************************/
/* Image Loaders                                                             */
/*****************************************************************************/
void
Texture::loadTexture2D(std::string filename, int target, int flag)
{
  SDL_Surface *image = IMG_Load(filename.c_str());

  if(image) {
    internalFormat = format = GL_RGB;
    switch(image->format->BytesPerPixel) {
    case 4:
      internalFormat = format = GL_RGBA;
      break;
    case 3:
      internalFormat = format = GL_RGB;
      break;
    case 1:
      internalFormat = format = GL_ALPHA;
      break;
    }

    type = GL_UNSIGNED_BYTE;
    SDL_LockSurface(image);
    if(flag&BUILD_MIPMAPS)
      gluBuild2DMipmaps(target, internalFormat, image->w, image->h,
                        format, type, image->pixels);
    else
      glTexImage2D(target, 0, internalFormat, image->w, image->h,
                   0, format, type, image->pixels);
    SDL_UnlockSurface(image);
    SDL_FreeSurface(image);
  } else
    throw new Exception("loadTexture2D: Can't load image");
}

void
Texture::loadCubemap(int flag)
{
  size_t p = name.rfind(".");
  std::string ext = name.substr(p);
  std::string base = name.substr(0,p);
  
  for(unsigned int i = 0; i < 6; i++)
    loadTexture2D(base+CUBEMAPSUFFIXE[i]+ext, GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT+i, flag);
}

/*****************************************************************************/
/* Satic variables                                                           */
/*****************************************************************************/
const std::string Texture::CUBEMAPSUFFIXE[6] = {"_posx", "_negx",
                                                "_posy", "_negy",
                                                "_posz","_negz"};
