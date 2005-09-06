#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <iostream>

#include "renderer.h"

namespace StarEngine {
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

//   bool
//   Renderer::printGLError()
//   {
// #ifndef NDEBUG
//     GLenum errcode;
//     if((errcode = glGetError()) != GL_NO_ERROR) {
//       cerr <<  "***glError: " << gluErrorString(errcode) << endl;
//       return true;
//     }
// #endif
//     return false;
//   }
  bool
  Renderer::printGLError2(const char *i, int l)
  {
#ifndef NDEBUG
    GLenum errcode;
    if((errcode = glGetError()) != GL_NO_ERROR) {
      cerr <<  "***glError: " << i << ":" << l << " " << gluErrorString(errcode) << endl;
      return true;
    }
#endif
    return false;
  }

  bool
  Renderer::printCGError2(const char *i, int l)
  {
#ifndef NDEBUG
    CGerror error = cgGetError();
    if(error != CG_NO_ERROR) {
      cerr <<  "***cgError: " << i << ":" << l << " " << cgGetErrorString(error) << endl;
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

  const char *
  Renderer::getCGErrorString()
  {
    CGerror error = cgGetError();
    if(error != CG_NO_ERROR)
      return (const char*)cgGetErrorString(error);
    else
      return "No error";
  }
}
