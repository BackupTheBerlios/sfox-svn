#include <SDL.h>
#include <iostream>

#include "timer.h"

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
