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
#include "framework/exception.h"

namespace StarEngine {
  using namespace std;

  Shader::Shader(ShaderType type)
    :type(type), program(0), vertexProgramARB(0), fragmentProgramARB(0)
  {
    switch(type) {
    case GLSL:
      program = glCreateProgramObjectARB();
      break;
    case ARB:
      break;
    default:
      assert(0);
    }
  }

  Shader::~Shader()
  {
    switch(type) {
    case GLSL:
      glDeleteObjectARB(program);
      break;
    case ARB:
      if(vertexProgramARB)
        glDeleteProgramsARB(1, &vertexProgramARB);
      if(fragmentProgramARB)
        glDeleteProgramsARB(1, &fragmentProgramARB);
      break;
    default:
      assert(0);
    }
  }

  void
  Shader::setVertexShader(const char *filename)
  {
    switch(type) {
    case GLSL:
      setShader(filename, GL_VERTEX_SHADER_ARB);
      break;
    case ARB:
      glGenProgramsARB(1, &vertexProgramARB);
      bind();
      loadProgramARB(filename, GL_VERTEX_PROGRAM_ARB);
      useFixedPipeline();
      break;
    default:
      assert(0);
    }
  }

  void
  Shader::setFragmentShader(const char *filename)
  {
    switch(type) {
    case GLSL:
      setShader(filename, GL_FRAGMENT_SHADER_ARB);
      break;
    case ARB:
      glGenProgramsARB(1, &fragmentProgramARB);
      bind();
      loadProgramARB(filename, GL_FRAGMENT_PROGRAM_ARB);
      useFixedPipeline();
      break;
    default:
      assert(0);
    }
  }

  void
  Shader::link() const
  {
    if(type != GLSL)
      return;
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
    switch(type) {
    case GLSL:
      glUseProgramObjectARB(program);
      break;
    case ARB:
      if(fragmentProgramARB) {
        glEnable(GL_FRAGMENT_PROGRAM_ARB);
        glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fragmentProgramARB);
      }
      if(vertexProgramARB) {
        glEnable(GL_VERTEX_PROGRAM_ARB);
        glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vertexProgramARB);
      }
      break;
    }
  }

  void
  Shader::useFixedPipeline()
  {
    glDisable(GL_FRAGMENT_PROGRAM_ARB);
    glDisable(GL_VERTEX_PROGRAM_ARB);
    glUseProgramObjectARB(0);
  }

  void
  Shader::validate() const
  {
    if(type != GLSL)
      return;
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

  void
  Shader::loadProgramARB(const char *filename, GLuint type)
  {
    const char *src = load(filename);
    int length = strlen(src);
    glProgramStringARB(type, GL_PROGRAM_FORMAT_ASCII_ARB, length, src);
    GLint errorPos;
    glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &errorPos);
    if(errorPos >= 0 && errorPos < length) {
      std::string mess("Program::load(): Can't compile program\nFile: ");
      mess.append(filename);
      mess.append("\n");
      mess.append((const char *)glGetString(GL_PROGRAM_ERROR_STRING_ARB));
      mess.append(src+errorPos);
      throw new Exception(mess);
    }
    delete[] src;
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
}
