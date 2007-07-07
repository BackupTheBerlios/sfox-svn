#ifndef UILOGGERDISPLAY_H
#define UILOGGERDISPLAY_H

#include "UIFramedWindow.h"

#include "Logger.h"

#include <GL/gl.h>

#include <iostream>

template <class T>
class UILoggerDisplay : public UIFramedWindow
{
public:
  UILoggerDisplay() : UIFramedWindow() { setBackground(1., 1., 1., 0.7); }

  void setLogger(LoggerSimple<T> *logger) { m_logger = logger; }

  virtual void draw()
  {
    UIFramedWindow::draw();
    Vec2i size = getSize();
    const typename LoggerSimple<T>::TimedValueList oldVl = m_logger->getValues();
    size_t numValues = oldVl.size();
    int start = 0;
    if(numValues > size[0])
      start = oldVl.size()-size[0];

    typename LoggerSimple<T>::TimedValueList vl;
    vl = remapValue(oldVl, start);
    numValues = vl.size();

    if(vl.empty())
      return;

    glColor3f(1, 0, 0);
//    glBegin(GL_LINE_STRIP);
    glBegin(GL_LINES);
//    glBegin(GL_POINTS);
    float x = size[0]-1;
    for(int i = (int)numValues-1; i >= 0 && x >= 0; i--, x--) {
      glVertex2f(x, 0);
      glVertex2f(x, vl[i].second);
      assert(vl[i].second>=0);
    }
    glEnd();
}

private:
  /**
   * Remap value between 0 and m_size[1]
   */
  typename LoggerSimple<T>::TimedValueList
  remapValue(const typename LoggerSimple<T>::TimedValueList &values, int first)
  {
    typename LoggerSimple<T>::TimedValueList res;
    Vec2i size = getSize();

    if(values.size() <= first)
      return res;

    T minV = values[first].second;
    T maxV = minV;
    for(size_t i = first; i < values.size(); i++)
    {
      maxV = std::max(values[i].second, maxV);
      minV = std::min(values[i].second, minV);
    }

    maxV = maxV;
    minV = minV;
    float scale = 0.f;
    if ( minV != maxV )
      scale = 0.8*size[1]/float(maxV-minV);

    //std::cerr << "scale: " << scale << "minV: " << minV << " maxV: " << maxV << std::endl;

    for ( size_t i = first; i < values.size(); i++)
    {
      typename LoggerSimple<T>::TimedValue v;
      v = std::make_pair(values[i].first, (values[i].second-minV)*scale);
      //std::cerr << v.second << std::endl;
      res.push_back(v);
    }
    return res;
  }

  LoggerSimple<T> *m_logger;
};


#endif
