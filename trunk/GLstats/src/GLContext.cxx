#include "GLContext.h"

#include <GL/glx.h>

#include "GlobalGLStat.h"

GLContext *
GLContext::getCurrent()
{
  GLXContext ctx = glXGetCurrentContext();
  size_t size = globalGLS.contextList.size();
  for(size_t i = 0; i < size; i++) {
    if(globalGLS.contextList[i]->m_glContext == ctx)
      return globalGLS.contextList[i];
  }
  abort();
}
