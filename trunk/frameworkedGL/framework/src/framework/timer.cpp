#include <SDL.h>
#include <iostream>

#include "timer.h"

namespace StarEngine {
  Timer::Timer()
  {
    start();
  }

  void
  Timer::start()
  {
    startTime = SDL_GetTicks();
  }

  unsigned int
  Timer::getElapsed()
  {
    return SDL_GetTicks()-startTime;
  }
}
