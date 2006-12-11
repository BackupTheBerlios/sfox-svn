#ifndef COUNTERMANAGER_H
#define COUNTERMANAGER_H

#include <map>

#include "Counter.h"

class CounterManager {
public:
  Counter *getCounter(std::string name) { return counters[name]; }
  void add(Counter *cnt) { counters[cnt->getName()] = cnt; }
  static CounterManager *getInstance();

private:
  static CounterManager *m_instance;
  std::map<std::string, Counter *> counters;

};

#define counterManager (*CounterManager::getInstance())

#endif
