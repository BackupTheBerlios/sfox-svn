#ifndef UIWINDOW_H
#define UIWINDOW_H

#include <vector>
#include "vector2.h"

#include "UIObject.h"

class UIWindow : public UIObject
{
public:
  UIWindow();

  virtual void draw();

  void setSize(int x, int y) { size = Vec2i(x, y); }
  void setPosition(int x, int y) { pos = Vec2i(x, y); }

private:
  void drawBackground();
  void beginDraw();
  void endDraw();

  typedef std::vector<UIObject *> UIObjectList;
  UIObjectList child;

  Vec2i size;
  Vec2i pos;
};

#endif
