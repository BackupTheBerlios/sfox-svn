#ifndef PROGRAM_H
#define PROGRAM_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

class Program {
private:
  GLuint name;
  unsigned int type; //Vp or fp
  
public:
  //if filename's extension = .vp or .fp
  Program(const char *filename) { load(filename); }
  ~Program();

  void bind();
  void load(const char *filename);
};

#endif
