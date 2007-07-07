#ifndef COUNTERMANAGER_H
#define COUNTERMANAGER_H

#include <map>
#include <string>

#include "Counter.h"
#include "Singleton.h"

class CounterManager : public Singleton<CounterManager>
{
public:
  Counter *getCounter(std::string name) { return m_counters[name]; }
  void add(Counter *cnt) { m_counters[cnt->getName()] = cnt; }

private:
  std::map<std::string, Counter *> m_counters;

};

#define counterManager (*CounterManager::getInstance())

#endif
