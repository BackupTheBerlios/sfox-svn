#include "UIFramedWindow.h"
#include "UIFrame.h"

#include <GL/gl.h>

UIFramedWindow::UIFramedWindow()
{
  m_frame = new UIFrame;
  m_frame->setParent(this);
}
/*******************************************************************************/

UIFramedWindow::~UIFramedWindow()
{
  delete m_frame;
  m_frame = NULL;
}
/*******************************************************************************/

void
UIFramedWindow::beginDraw()
{
  UIWindow::beginDraw();
  Vec2i bSize = m_frame->getBorderSize();
  glPushMatrix();
  glTranslatef(bSize[0], bSize[1], 0.0f);
}
/*******************************************************************************/

void
UIFramedWindow::endDraw()
{
  glPopMatrix();
  m_frame->setSize(m_size);
  m_frame->draw();
  UIWindow::endDraw();
}
