#include "GLXFuncs.h"

#include <stdio.h>

#include "glstats.h"

#include "timer.h"
#include "GlobalGLStat.h"

void
__attribute__ ((constructor)) sharedLibInit()
{
  GLStatsInit();
}

void
__attribute__ ((destructor)) sharedLibExit()
{
  GLStatsExit();
}

void
glXSwapBuffers( Display *dpy, GLXDrawable drawable )
{
  GLStatsSwapBuffers();
  glXFuncs->glXSwapBuffers(dpy, drawable);
}

GLXContext
glXCreateContext(Display *dpy, XVisualInfo *vis,
                 GLXContext shareList, Bool direct)
{
  GLStatsInitGL();
  GLXContext context;
  context = glXFuncs->glXCreateContext(dpy, vis, shareList, direct);

  globalGLS.contextList.push_back(new GLContext(context));

  return context;
}
