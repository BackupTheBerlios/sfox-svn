#include <stdlib.h>

#include "stargl.h"

#include "viewport.h"

struct viewport {
  int x, y;
  int width, height;
};

viewport
viewport_create(int x, int y, int width, int height)
{
  viewport vp = (struct viewport *)malloc(sizeof(struct viewport));

  vp->x = x;
  vp->y = y;
  vp->width = width;
  vp->height = height;

  return vp;
}

void
viewport_destroy(viewport vp)
{
  free(vp);
}

double
viewport_ratio(viewport vp)
{
  return (double)vp->width/vp->height;
}

void
viewport_to_opengl(viewport vp)
{
  glViewport(vp->x, vp->y, vp->width, vp->height);
}
