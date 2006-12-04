#ifndef UIWINDOW_H
#define UIWINDOW_H

#include <vector>
#include "vector2.h"

#include "UIObject.h"

class UIWindow : public UIObject
{
public:
  /**
   * Create a window of size(200, 100) and pos(0,0)
   */
  UIWindow();

  /**
   * Draw the window and all its children
   */
  virtual void draw();

  /**
   * Set the size of the window in pixel
   */
  void setSize(int x, int y) { m_size = Vec2i(x, y); }

  /**
   * Set the position of the window in pixel.
   * (0,0) is (left,bottom)
   */
  void setPosition(int x, int y) { m_pos = Vec2i(x, y); }

  /**
   * Display at the center of the screen. setPosition() is ignored
   */
  void setCenter(bool f) { m_center = f; }

private:
  void drawBackground();
  void beginDraw();
  void endDraw();

  typedef std::vector<UIObject *> UIObjectList;
  UIObjectList child;

  bool m_center;
  Vec2i m_size;
  Vec2i m_pos;

  static const Vec2i DEFAULT_SIZE;
  static const Vec2i DEFAULT_POS;
};

#endif
