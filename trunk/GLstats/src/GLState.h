#ifndef GLSTATE_H
#define GLSTATE_H

#include <vector>

class GLState {
public:
  GLState():m_curFrame(0) {}

  /**
   * Increase the current frame
   */
  void incCurFrame() { m_curFrame++; }
  size_t getCurFrame() { return m_curFrame; }

private:
  size_t m_curFrame;
};

#endif
