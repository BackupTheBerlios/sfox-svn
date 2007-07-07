#include "GlobalGLStat.h"

#include "vector2.h"

void
GlobalGLStat::beginGLStatsDrawing()
{
  GLint maxUnits;
  glGetIntegerv(GL_MAX_TEXTURE_UNITS, &maxUnits);

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  for(int i = maxUnits-1; i >= 0; i--) {
    glActiveTexture(GL_TEXTURE0+i);
    glDisable(GL_TEXTURE_3D);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_1D);
  }
  glDisable(GL_LIGHTING);
  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glDisable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  Vec2i vpSize(vp[2], vp[3]);
  Vec2i vpPos(vp[0], vp[1]);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(vpPos[0], vpSize[0]-1, vpPos[1], vpSize[1]-1, -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
}/*****************************************************************************/

void
GlobalGLStat::endGLStatsDrawing()
{
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();
}/*****************************************************************************/
