#include "GLContext.h"

#include <GL/glx.h>

#include "Global.h"

GLContext *
GLContext::getCurrent()
{
  GLXContext ctx = glXGetCurrentContext();
  size_t size = global.contextList.size();
  for(size_t i = 0; i < size; i++) {
    if(global.contextList[i]->m_glContext == ctx)
      return global.contextList[i];
  }
  abort();
}
