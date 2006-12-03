#include "UIWindow.h"

#include <GL/gl.h>

UIWindow::UIWindow()
{

}/*****************************************************************************/

void
UIWindow::draw()
{
  beginDraw();

  drawBackground();

  UIObjectList::iterator it;
  UIObjectList::iterator end = child.end();
  for(it = child.begin(); it != end; ++it)
    (*it)->draw();
  endDraw();
}/*****************************************************************************/

void UIWindow::drawBackground()
{
  glColor3f(1, 1, 1);
  glBegin(GL_QUADS);
  glVertex2f(0, 0);
  glVertex2f(size[0], 0);
  glVertex2f(size[0], size[1]);
  glVertex2f(0, size[1]);
  glEnd();
}/*****************************************************************************/

void
UIWindow::beginDraw()
{
  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(vp[0], vp[2], vp[1], vp[3], -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glTranslatef(pos[0], pos[1], 0.0f);
}/*****************************************************************************/

void UIWindow::endDraw()
{
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}/*****************************************************************************/
