#include "Global.h"

void
Global::beginGLStatsDrawing()
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
}/*****************************************************************************/

void
Global::endGLStatsDrawing()
{
  glPopAttrib();
}/*****************************************************************************/
