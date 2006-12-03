#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>

#include "GLContext.h"

class Global {
public:
  std::vector<GLContext *> contextList;

  static Global *getInstance();

  void beginGLStatsDrawing();
  void endGLStatsDrawing();

private:
  static Global *m_instance;
};

#define global (*Global::getInstance())

#endif
