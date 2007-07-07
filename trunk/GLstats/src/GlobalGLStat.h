#ifndef GLOBALGLSTAT_H
#define GLOBALGLSTAT_H

#include <vector>

#include "GLContext.h"
#include "Singleton.h"

class GlobalGLStat : public Singleton<GlobalGLStat>
{
public:
  std::vector<GLContext *> contextList;

  void beginGLStatsDrawing();
  void endGLStatsDrawing();
};

#define globalGLS (*GlobalGLStat::getInstance())

#endif
