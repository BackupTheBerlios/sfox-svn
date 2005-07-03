#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

#include "renderer.h"

using namespace std;

void
Renderer::printModelView()
{
#ifndef NDEBUG
  float m[4][4];
  glGetFloatv(GL_MODELVIEW_MATRIX, m[0]);
  for(int j=0;j<4;j++) {
    for(int i=0;i<4;i++)
      cout << m[i][j] << " ";
    cout << "\n";
  }
#endif
}

bool
Renderer::printGLError()
{
#ifndef NDEBUG
  GLenum errcode;
  if((errcode = glGetError()) != GL_NO_ERROR) {
    cerr <<  "***glError: " << gluErrorString(errcode) << endl;
    return true;
  }  
#endif
  return false;
}

const char *
Renderer::getGLErrorString()
{
  GLenum errcode;
  if((errcode = glGetError()) != GL_NO_ERROR)    
    return (const char *)gluErrorString(errcode);
  else
    return "No error";
}
