#ifndef TIMER_H
#define TIMER_H

namespace StarEngine {
  class Timer {
  private:
    unsigned int startTime;

  public:
    Timer();
    void start();
    unsigned int getElapsed();
  };
}
#endif
