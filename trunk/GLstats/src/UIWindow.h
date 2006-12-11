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
   * Display at the center of the screen. setPosition() is ignored
   */
  void setCenter(bool f) { m_center = f; }

  void addChild(UIObject *obj) { child.push_back(obj); }

private:
  void drawBackground();
  void beginDraw();
  void endDraw();

  typedef std::vector<UIObject *> UIObjectList;
  UIObjectList child;

  bool m_center;

  static const Vec2i DEFAULT_SIZE;
  static const Vec2i DEFAULT_POS;
};

#endif
