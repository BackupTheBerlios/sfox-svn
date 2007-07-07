#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>

#include "GLContext.h"
#include "Singleton.h"

class Global : public Singleton<Global>
{
public:
  std::vector<GLContext *> contextList;

  void beginGLStatsDrawing();
  void endGLStatsDrawing();
};

#define global (*Global::getInstance())

#endif
