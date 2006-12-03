#include "timer.h"

#include <iostream>
#include <cassert>

bool Timer::isInit = false;
struct timeval Timer::initialTicks;


Timer::Timer()
{
  start();
}

void
Timer::init()
{
  if(!isInit) {
    struct timeval now;
    gettimeofday(&now, NULL);
    isInit = true;
  }
}

void
Timer::start()
{
  startTime = getTicks();
}

uint64_t
Timer::getTicks()
{
  assert(isInit);
  uint64_t ticks;
  struct timeval now;
  gettimeofday(&now, NULL);
  ticks = (now.tv_sec-initialTicks.tv_sec)*1000000+(now.tv_usec-initialTicks.tv_usec);
  return ticks;
}

uint64_t
Timer::getElapsed()
{
  return getTicks()-startTime;
}
