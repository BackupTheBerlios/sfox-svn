#ifdef _WIN32
#include <windows.h>
#endif
#include <cassert>
#include <fstream>
#include <string>
#include <GL/gl.h>
#include <GL/glext.h>

#include "shader.h"
#include "glext.h"
#include "exception.h"

using namespace std;

Shader::Shader()
{
  program = glCreateProgramObjectARB();
}

Shader::~Shader()
{
  glDeleteObjectARB(program);
}

void
Shader::setVertexShader(const char *filename)
{
  setShader(filename, GL_VERTEX_SHADER_ARB);
}

void
Shader::setFragmentShader(const char *filename)
{
  setShader(filename, GL_FRAGMENT_SHADER_ARB);
}

void
Shader::link() const
{
  GLint linked = 0;
  glLinkProgramARB(program);
  glGetObjectParameterivARB(program, GL_OBJECT_LINK_STATUS_ARB, &linked);
  if(!linked) {
    GLcharARB *log = getLog(program);
    std::string mess("Shader::link(): Can't link");
    if(log) {
      mess.append(":\nLog:\n");
      mess.append(log);
    }
    delete[] log;
    throw new Exception(mess);
  }
}

void
Shader::bind() const
{
  glUseProgramObjectARB(program);
}

void
Shader::useFixedPipeline()
{
  glUseProgramObjectARB(0);
}

void
Shader::validate() const
{
  GLint validated = 0;
  glValidateProgramARB(program);
  glGetObjectParameterivARB(program, GL_OBJECT_VALIDATE_STATUS_ARB,
                            &validated);
  if(!validated) {
    GLcharARB *log = getLog(program);
    std::string mess("Shader::validate(): Can't validate shader:");
    if(log) {
      mess.append("\nLog:\n");
      mess.append(log);
    }
    delete[] log;
    throw new Exception(mess);
  }
}

/*****************************************************************************/
/* Uniform methods                                                           */
/*****************************************************************************/
void
Shader::setUniform(const char *name, float a)
{
  glUniform1fARB(getUniform(name), a);
}

void
Shader::setUniform(const char *name, int a)
{
  glUniform1iARB(getUniform(name), a);
}

void
Shader::setUniform(const char *name, float a, float b, float c)
{
  glUniform3fARB(getUniform(name), a, b, c);
}

GLint
Shader::getUniform(const char *name)
{
  map<const char*, GLint>::iterator it = uniforms.find(name);
  if(it == uniforms.end()) {
    uniforms[name] = glGetUniformLocationARB(program, name);
    assert(uniforms[name]!=-1);
  }
  return uniforms[name];
}

GLint
Shader::getAttrib(const char *name)
{
  GLint attrib = glGetAttribLocationARB(program, name);
  if(attrib == -1) {
    std::string mess("Shader::getAttrib(): Can't get location of attrib \"");
    mess.append(name);
    throw new Exception(mess.append("\"."));
  } 
  return attrib;
}

void
Shader::bindAttribLocation(int i, const char *name) const
{
  glBindAttribLocationARB(program, 0, name);
}

/*****************************************************************************/
/* Helper function                                                           */
/*****************************************************************************/
void
Shader::setShader(const char *filename, GLenum type)
{
  const char *source = load(filename);
  GLuint shader = glCreateShaderObjectARB(type);
  glShaderSourceARB(shader, 1, &source, NULL);
  glCompileShaderARB(shader);

  GLint compiled = 0;
  glGetObjectParameterivARB(shader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
  if(!compiled) {
    GLcharARB *log = getLog(shader);
    std::string mess("Shader::setShader(): Can't compile \"");
    mess.append(filename);
    mess.append("\"");
    if(log) {
      mess.append("\nLog:\n");
      mess.append(log);
    }
    delete[] log;
    throw new Exception(mess);
  }

  glAttachObjectARB(program, shader);
  glDeleteObjectARB(shader);

  delete[] source;
}

GLcharARB *
Shader::getLog(GLuint shader) const
{
  int length = 0;
  GLcharARB *str = 0;
  glGetObjectParameterivARB(shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
  if(length>1) {
    str = new GLcharARB[length];
    glGetInfoLogARB(shader, length, &length, str);
    str[length-1] = '\0';
  }
  return str;
}

char *
Shader::load(const char *filename)
{
  int length;
  char *text;
  fstream fs;

  //Load the source as a null terminated string
  fs.open(filename, fstream::in);
  if(!fs.is_open()) {
    string mess("Shader::load(): Can't load program: ");
    throw new Exception(mess.append(filename));
  }
     
  fs.seekg(0, ios_base::end);
  length = fs.tellg();
  fs.seekg (0, ios::beg);
  text = new char[length+1];
  fs.read (text, length);
  fs.close();
  text[length] = '\0';

  return text;
}
