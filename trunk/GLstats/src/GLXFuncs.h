#ifndef GLXFUNCS_H
#define GLXFUNCS_H

#include <GL/glx.h>

#include "Singleton.h"

typedef void(*PFNGLXSWAPBUFFERS)(Display*, GLXDrawable);
typedef GLXContext(*PFNGLXCREATECONTEXT)(Display*, XVisualInfo*, GLXContext, Bool);

class GLXFuncs : public Singleton<GLXFuncs>
{
public:
  GLXFuncs();

  void (*glXSwapBuffers)(Display*,GLXDrawable);
  GLXContext (*glXCreateContext)(Display *dpy, XVisualInfo *vis,
                                 GLXContext shareList, Bool direct);
};

#define glXFuncs GLXFuncs::getInstance()

#endif
