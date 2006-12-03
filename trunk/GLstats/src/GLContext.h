#ifndef GLCONTEXT_H
#define GLCONTEXT_H

#include <GL/glx.h>

#include "GLState.h"

class GLContext {
public:
  GLContext(GLXContext context):m_glContext(context) {}
  GLState *getState() { return &m_state; }

  static GLContext *getCurrent();

private:
  GLState m_state;
  GLXContext m_glContext;
};

#endif
