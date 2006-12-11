#ifndef UIOBJECT_H
#define UIOBJECT_H

#include "vector2.h"

class UIObject {
public:
  virtual ~UIObject() {}

  /**
   * Draw the window and all its children
   */
  virtual void draw() = 0;

  /**
   * Set the size of the window in pixel
   */
  virtual void setSize(int x, int y) { m_size = Vec2i(x, y); }

  /**
   * Set the position of the window in pixel.
   * (0,0) is (left,bottom)
   */
  virtual void setPosition(int x, int y) { m_pos = Vec2i(x, y); }

protected:
  Vec2i m_size;
  Vec2i m_pos;
};

#endif
