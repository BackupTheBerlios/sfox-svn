#include "GLXFuncs.h"

#include "LibraryLoader.h"

GLXFuncs *GLXFuncs::instance = 0;

GLXFuncs *
GLXFuncs::getInstance()
{
  if(!instance)
    instance = new GLXFuncs();
  return instance;
}

GLXFuncs::GLXFuncs()
{
  LibraryLoader loader;
  loader.open("libGL.so");
  GLXFuncs::glXSwapBuffers = (PFNGLXSWAPBUFFERS)loader.getSym("glXSwapBuffers");
  GLXFuncs::glXCreateContext = (PFNGLXCREATECONTEXT)loader.getSym("glXCreateContext");
}
