#ifndef GLXFUNCS_H
#define GLXFUNCS_H

#include <GL/glx.h>

typedef void(*PFNGLXSWAPBUFFERS)(Display*, GLXDrawable);
typedef GLXContext(*PFNGLXCREATECONTEXT)(Display*, XVisualInfo*, GLXContext, Bool);

class GLXFuncs {
public:
  static GLXFuncs *getInstance();

  void (*glXSwapBuffers)(Display*,GLXDrawable);
  GLXContext (*glXCreateContext)(Display *dpy, XVisualInfo *vis,
                                 GLXContext shareList, Bool direct);
private:
  GLXFuncs();

  static GLXFuncs *instance;
};

#define glXFuncs GLXFuncs::getInstance()

#endif
