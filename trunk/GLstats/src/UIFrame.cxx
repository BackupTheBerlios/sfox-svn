#include "UIFrame.h"

#include <GL/gl.h>

UIFrame::UIFrame()
  : UIObject()
{
  m_borderSize = Vec2i(1, 1);
}
/*******************************************************************************/

void
UIFrame::draw()
{
  Vec2i size = m_size;
  glColor3f(0.f, 0.f, 1.0f);
  glBegin(GL_LINE_STRIP);
  glVertex2f(0.f, 0.f);
  glVertex2f(size[0]-1, 0.f);
  glVertex2f(size[0]-1, size[1]-1);
  glVertex2f(0.f, size[1]-1);
  glVertex2f(0.f, 0.f);
  glEnd();
}
/*******************************************************************************/
