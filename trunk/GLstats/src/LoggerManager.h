#ifndef LOGGERMANAGER_H
#define LOGGERMANAGER_H

#include <map>

#include "Logger.h"

class LoggerManager {
public:
  Logger *getLogger(std::string name) { return loggers[name]; }
  void add(Logger *log) { loggers[log->getName()] = log; }
  static LoggerManager *getInstance();

private:
  static LoggerManager *m_instance;
  std::map<std::string, Logger *> loggers;
};

#define loggerManager (*LoggerManager::getInstance())

#endif
