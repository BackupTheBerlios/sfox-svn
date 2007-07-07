#ifndef LOGGERMANAGER_H
#define LOGGERMANAGER_H

#include <map>

#include "Logger.h"
#include "Singleton.h"

class LoggerManager : public Singleton<LoggerManager>
{
public:
  Logger *getLogger(std::string name) { return m_loggers[name]; }
  void add(Logger *log) { m_loggers[log->getName()] = log; }

private:
  std::map<std::string, Logger *> m_loggers;
};

#define loggerManager (*LoggerManager::getInstance())

#endif
