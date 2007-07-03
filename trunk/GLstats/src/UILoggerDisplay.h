#ifndef UILOGGERDISPLAY_H
#define UILOGGERDISPLAY_H

#include "UIObject.h"

#include "Logger.h"

#include <GL/gl.h>

template <class T>
class UILoggerDisplay : public UIObject {
public:

  void setLogger(LoggerSimple<T> *logger) { m_logger = logger; }

  virtual void draw()
  {
    int space = m_size[1];
    const std::vector<std::pair<uint64_t, T> > vl = m_logger->getValues();
    T vMax = 1;
    int start = 0;
    if(vl.size() > m_size[0])
      start = vl.size()-m_size[0];
    for(int i = start; i < vl.size(); i++) {
      if(vMax < vl[i].second)
        vMax = vl[i].second;
    }

    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    float scale = m_size[1]/float(vMax);
    float x = m_size[0];
    for(int i = vl.size(); i >= 0 && x >= 0; i--, x--) {
      //glVertex2f(x, 0);
      glVertex2f(x, vl[i].second*scale);
    }
    glEnd();
  }

private:
  LoggerSimple<T> *m_logger;
};


#endif
