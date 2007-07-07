#ifndef UIFRAME_H
#define UIFRAME_H

#include "UIObject.h"

class UIFrame : public UIObject
{
public:
  UIFrame();
  virtual void draw();
  /**
   * Return size of side and top borders
   */
  Vec2i getBorderSize() const { return m_borderSize; }

private:
  Vec2i m_borderSize;
};

#endif
