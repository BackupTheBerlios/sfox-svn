#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <string>
#include <utility>

#include "timer.h"

/**
 * Base logger class
 */
class Logger
{
public:
  /**
   * Create a logger with name name
   */
  Logger(const char *name):m_name(name) {}

  /**
   * Start to overwrite previous values when reaching m_maxEntries
   */
  void setMaxNumEntries(size_t max) { m_maxEntries = max; }

  std::string getName() { return m_name; }
protected:
  std::string m_name;
  size_t m_maxEntries;
};

/**
 * Save a value and the time at which it has been added
 */
template <class T>
class LoggerSimple : public Logger
{
public:
  typedef std::pair<uint64_t, T> TimedValue;
  typedef std::vector<TimedValue> TimedValueList;
  typedef typename TimedValueList::iterator TimedValueListIterator;

  LoggerSimple(const char *name):Logger(name) {}

  void addValue(uint64_t time, T val) {
    m_values.push_back(TimedValue(time, val));
  }

  T getLastValue()
  {
    if(!m_values.empty())
      return m_values.back().second;
    return T(0);
  }

  /**
   * Returns all values
   */
  const std::vector<TimedValue> &getValues() const { return m_values; }

protected:
  std::vector<TimedValue> m_values;
};

#endif
