#include "GLFuncs.h"

#define ADDGLFUN(funcName, ret)                         \
  GLFuncs<ret>::getInstance()->addFunction(#funcName);

void
GLFuncsInit()
{
  #include "generated/glfun.h"
}
