#ifndef UIFRAMED_WINDOW_H
#define UIFRAMED_WINDOW_H

#include "UIWindow.h"

class UIFrame;

class UIFramedWindow : public UIWindow
{
public:
  UIFramedWindow();
  ~UIFramedWindow();

  void beginDraw();
  void endDraw();

protected:
  UIFrame* m_frame;
};

#endif
