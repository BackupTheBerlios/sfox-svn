#ifndef GLFUNCS_H
#define GLFUNCS_H

#include <GL/gl.h>
#include <map>
#include <string>

#include "Singleton.h"
#include "LibraryLoader.h"

template<typename T>
class GLFuncs : public Singleton< GLFuncs<T> >
{
public:
  typedef T (*GLFun)(...);

  GLFuncs()
  {
    addFunction("glBegin");
  }

  GLFun getFunction(const std::string& funcName) { return m_funcs[funcName]; }

  void addFunction(const std::string& name)
  {
    LibraryLoader loader;
    loader.open("libGL.so");
    m_funcs[name] = (GLFun)LibraryLoader::getNextSym(name.c_str());
  }

private:
  std::map<std::string, GLFun> m_funcs;
};

/**
 * Load all gl functions in GLFuns
 */
extern void GLFuncsInit();

//#define glFuncs GLFuncs<T>::getInstance()

#endif
