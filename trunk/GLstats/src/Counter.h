#ifndef COUNTER_H
#define COUNTER_H

#include "timer.h"

#include <string>

class Counter {
public:
  Counter(const char *name):m_name(name) {
    reset();
  }

  void reset() {
    m_timer.start();
    m_value = 0;
  }

  size_t getValue() { return m_value; }

  uint64_t getLastResetTime() { return m_timer.getElapsed(); }

  std::string getName() { return m_name; }

  void incValue() { m_value++; }

protected:
  std::string m_name;
  size_t m_value;

private:
  Timer m_timer;
};

#endif
