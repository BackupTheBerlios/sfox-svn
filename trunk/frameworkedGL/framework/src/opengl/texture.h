#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <utility>
#include <list>
#include <string>
#include <GL/gl.h>

#include "glext.h"
#include "math/matrix.h"

class Texture {  
public:

  enum {
    //target
    TEXTURE_2D = GL_TEXTURE_2D,
    TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP_ARB,
    TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE_ARB
  };
  
  enum {
    //format
    ALPHA = 1 << 0,
    RGB = 1 << 1,
    RGBA = 1 << 2,
    LUMINANCE = 1 << 3,

    //type
    UNSIGNED_BYTE = 1 << 4,
    FLOAT = 1 << 5,

    //filters
    NEAREST = 1 << 6,
    LINEAR = 1 << 7,
    NEAREST_MIPMAP_NEAREST = 1 << 8,
    LINEAR_MIPMAP_NEAREST = 1 << 9,
    NEAREST_MIPMAP_LINEAR = 1 << 10,
    LINEAR_MIPMAP_LINEAR = 1 << 11,
    BUILD_MIPMAPS = 1 << 12    
  };
  
  Texture::Texture(const char *filename, GLuint target = TEXTURE_2D,
                   int flag = RGB|UNSIGNED_BYTE);
  Texture::Texture(int width, int height, GLuint target = TEXTURE_2D,
                   int flag = RGB|UNSIGNED_BYTE);
  ~Texture();

  void bind();
  void copyTexSubImage2D(int xoffset, int yoffset, int x, int y, int width,
                         int height);
  /*only format bits are allowed in flag*/
  void subImage(const void *pixels, int xoffset, int yoffset, int width,
                int height, int flag);

  Matrix4f &getMatrix() { return m; }
  
  void texParameter(GLenum pname, GLuint param);

  void setTexGen(GLenum coord, GLint mode);
  void setTexEnv(GLenum pname, GLint param);
  void setTexEnv(GLenum pname, GLfloat param);

  static void setTextureStage(int n) { glActiveTextureARB(GL_TEXTURE0_ARB+n); }
  
private:
  Matrix4f m;
  std::string name;
  GLuint texName;

  /*Texture parameters extracted from flag*/
  int target;
  int format;
  int internalFormat;
  int type;
  int minFilter, magFilter;

  //_posx _negx...
  static const std::string CUBEMAPSUFFIXE[6];

  void loadCubemap(int flag);
  void loadTexture2D(std::string filename, int target, int flag);

  void genTexture();
  void extractParams(int flag);
};

#endif
