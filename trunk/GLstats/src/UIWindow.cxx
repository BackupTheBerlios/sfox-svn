#include "UIWindow.h"

#include <iostream>

#include <GL/gl.h>

const Vec2i UIWindow::DEFAULT_SIZE = Vec2i(200, 100);
const Vec2i UIWindow::DEFAULT_POS = Vec2i(0, 0);


UIWindow::UIWindow()
{
  m_size = Vec2i(DEFAULT_SIZE);
  m_pos = Vec2i(DEFAULT_POS);
  m_center = false;
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
  glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
  glBegin(GL_QUADS);
  glVertex2f(0, 0);
  glVertex2f(m_size[0], 0);
  glVertex2f(m_size[0], m_size[1]);
  glVertex2f(0, m_size[1]);
  glEnd();
}/*****************************************************************************/

void
UIWindow::beginDraw()
{
  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  Vec2i vpSize(vp[2], vp[3]);
  Vec2i vpPos(vp[0], vp[1]);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(vpPos[0], vpSize[0], vpPos[1], vpSize[1], -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  if(m_center)
    m_pos = (vpSize-m_size)/2;
  glTranslatef(m_pos[0], m_pos[1], 0.0f);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}/*****************************************************************************/

void UIWindow::endDraw()
{
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}/*****************************************************************************/
