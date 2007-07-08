#include "UIWindow.h"

#include <iostream>

#include <GL/gl.h>

const Vec2i UIWindow::DEFAULT_SIZE = Vec2i(200, 100);
const Vec2i UIWindow::DEFAULT_POS = Vec2i(0, 0);
const Vec4f UIWindow::DEFAULT_BG = Vec4f(0, 0, 0, 1.f);


UIWindow::UIWindow()
{
  m_size = Vec2i(DEFAULT_SIZE);
  m_pos = Vec2i(DEFAULT_POS);
  m_center = false;
  m_fullscreen = false;
  m_bgColor = DEFAULT_BG;
}/*****************************************************************************/

UIWindow::~UIWindow()
{
  UIObjectList::iterator it;
  UIObjectList::iterator end = m_child.end();
  for(it = m_child.begin(); it != end; ++it)
    delete (*it);
}/*****************************************************************************/

void
UIWindow::draw()
{
  UIObjectList::iterator it;
  UIObjectList::iterator end = m_child.end();
  for(it = m_child.begin(); it != end; ++it)
  {
    (*it)->beginDraw();
    (*it)->draw();
    (*it)->endDraw();
  }
}/*****************************************************************************/

void UIWindow::drawBackground()
{
  Vec2i size = getSize();
  glColor4fv(&m_bgColor[0]);
  glBegin(GL_QUADS);
  glVertex2f(0, 0);
  glVertex2f(size[0]-1, 0);
  glVertex2f(size[0]-1, size[1]-1);
  glVertex2f(0, size[1]-1);
  glEnd();
}/*****************************************************************************/

void
UIWindow::beginDraw()
{
  glPushAttrib(GL_SCISSOR_BIT);
  glEnable(GL_SCISSOR_TEST);
  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  Vec2i vpSize(vp[2], vp[3]);
  Vec2i vpPos(vp[0], vp[1]);

  if(m_fullscreen)
    setSize(vpSize);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  if(m_center)
    m_pos = (vpSize-m_size)/2;
  glTranslatef(m_pos[0], m_pos[1], 0.0f);

  Vec2i screenPos = getScreenPos();
  Vec2i size = m_size;
  glScissor(screenPos[0], screenPos[1], size[0], size[1]);

  drawBackground();
}/*****************************************************************************/

void
UIWindow::endDraw()
{
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();
}/*****************************************************************************/

void
UIWindow::addChild(UIObject *obj)
{
  obj->setParent(this);
  m_child.push_back(obj);
}/*****************************************************************************/
