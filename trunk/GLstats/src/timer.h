#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <sys/time.h>

/**
 * A timer class
 */
class Timer {
public:
  Timer();
  void start();

  /**
   * Returns time In microsecondes
   */
  uint64_t getElapsed();

  /**
   * This function need to be called before using timers
   */
  static void init();

  /**
   * Return the number of nanosecondes since init() was called
   */
  static uint64_t getTicks();

  static float toSec(uint64_t t) { return (float)t/1e6f; }

private:
  uint64_t startTime;

  static bool isInit;
  static struct timeval initialTicks;
};
#endif
