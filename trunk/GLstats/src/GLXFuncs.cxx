#include "GLXFuncs.h"

#include "LibraryLoader.h"

GLXFuncs::GLXFuncs()
{
  LibraryLoader loader;
  loader.open("libGL.so");
  GLXFuncs::glXSwapBuffers = (PFNGLXSWAPBUFFERS)loader.getSym("glXSwapBuffers");
  GLXFuncs::glXCreateContext = (PFNGLXCREATECONTEXT)loader.getSym("glXCreateContext");
}
