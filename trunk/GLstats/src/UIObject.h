#ifndef UIOBJECT_H
#define UIOBJECT_H

#include "vector2.h"

class UIObject {
public:
  UIObject(UIObject* parent = NULL) { m_parent = parent; }
  virtual ~UIObject() {}

  /**
   * Draw the window and all its children
   */
  virtual void draw() = 0;

  /**
   * Set the size of the window in pixel
   */
  virtual void setSize(int x, int y) { setSize(Vec2i(x, y)); }
  virtual void setSize(const Vec2i& s) { m_size = s; }

  /**
   * Set the position of the window in pixel.
   * (0,0) is (left,bottom)
   */
  virtual void setPosition(int x, int y) { m_pos = Vec2i(x, y); }

  /**
   * Return object size
   */
  virtual Vec2i getSize() { return m_size; }

  /**
   * Return object pos in parent coordinate
   */
  virtual Vec2i getPos() { return m_pos; }

  /**
   * Return object's  parent
   */
  UIObject* getParent() const { return m_parent; }

  /**
   * Set object's parent
   */
  virtual UIObject* setParent(UIObject* parent) { m_parent = parent; }

  /**
   * Called before draw
   */
  virtual void beginDraw() {}

  /**
   * Called after draw
   */
  virtual void endDraw() {}

  /**
   * Return position in screen space
   */
  virtual Vec2i getScreenPos() const
  {
    UIObject* parent = getParent();
    if(parent)
      return m_pos+parent->getScreenPos();
    return m_pos;
  }

protected:
  Vec2i m_size;
  Vec2i m_pos;
  UIObject* m_parent;
};

#endif
