#ifndef SHADER_H
#define SHADER_H
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <map>

class Shader {
private:
  std::map<const char*, GLint> uniforms;
  GLuint program;

  void setShader(const char *filename, GLenum type);
  char *load(const char *filename);
  GLint getUniform(const char *name);

  GLcharARB * getLog(GLuint shader) const;

public:
  Shader();
  ~Shader();
  
  void setFragmentShader(const char *filename);
  void setVertexShader(const char *filename);

  //Uniforms
  void setUniform(const char *name, float a);
  void setUniform(const char *name, float a, float b, float c);
  void setUniform(const char *name, int a);

  //Attribs
  GLint getAttrib(const char *name);
  void bindAttribLocation(int i, const char *name) const;

  
  void link() const;
  void bind() const;
  static void useFixedPipeline();

  void validate() const;
};

#endif
