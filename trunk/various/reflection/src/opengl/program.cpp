#ifdef _WIN32
#include <windows.h>
#endif

#include <fstream>
#include <iostream>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>

#include "program.h"
#include "glext.h"
#include "exception.h"

using namespace std;

void
Program::load(const char *filename)
{
  int length;
  char *text;
  fstream fs;

  //Find if it's a vp or a fp
  size_t len = strlen(filename);
  type = 0;
  if(len>3) {
    if(strncmp(".vp", filename+len-3, 3)==0)
      type = GL_VERTEX_PROGRAM_ARB;
    else if(strncmp(".fp", filename+len-3, 3)==0)
      type = GL_FRAGMENT_PROGRAM_ARB;
  }
  
  if(!type) {
    string mess("Program: Don't know what is: ");
    throw new Exception(mess.append(filename));
  }

  //Load the source
  fs.open(filename, fstream::in);
  if(!fs.is_open()) {
    string mess("Program: Can't load program: ");
    throw new Exception(mess.append(filename));
  }
     
  fs.seekg(0, ios_base::end);
  length = fs.tellg();
  fs.seekg (0, ios::beg);
  text = new char[length];
  fs.read (text, length);
  fs.close();

  //Send it to OpenGL
  glGenProgramsARB(1, &name);
  bind();
  glProgramStringARB(type, GL_PROGRAM_FORMAT_ASCII_ARB, length, text);

  delete[] text;
}

Program::~Program()
{
  glDeleteProgramsARB(1, &name);
}

void
Program::bind()
{
  glBindProgramARB(type, name);
}
