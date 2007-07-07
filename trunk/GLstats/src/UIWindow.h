#ifndef UIWINDOW_H
#define UIWINDOW_H

#include <vector>
#include "vector2.h"
#include "vector4.h"

#include "UIObject.h"

class UIWindow : public UIObject
{
public:
  /**
   * Create a window of size(200, 100) and pos(0,0)
   */
  UIWindow();

  /**
   * Delete all children
   */
  ~UIWindow();

  /**
   * Draw the window and all its children
   */
  virtual void draw();

  /**
   * Display at the center of the screen. setPosition() is ignored
   */
  void setCenter(bool f) { m_center = f; }

  /**
   * Add object to the window. Deleted when this window is deleted
   */
  void addChild(UIObject *obj);

  /**
   * Set fullscreen mode
   */
  void setFullScreen(bool flag) { m_fullscreen = flag; }

  /**
   * Set background color
   */
  void setBackground(const Vec4f &color) { m_bgColor = color; }
  void setBackground(float r, float g, float b, float a) { setBackground(Vec4f(r,g,b,a)); }

  virtual void beginDraw();
  virtual void endDraw();

private:
  void drawBackground();

  typedef std::vector<UIObject *> UIObjectList;
  UIObjectList m_child;

  bool m_center;
  bool m_fullscreen;
  Vec4f m_bgColor;

  static const Vec2i DEFAULT_SIZE;
  static const Vec2i DEFAULT_POS;
  static const Vec4f DEFAULT_BG;
};

#endif
