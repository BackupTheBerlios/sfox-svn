#ifndef LOGGERXML_H
#define LOGGERXML_H

#include "Logger.h"

class LoggerXml : public Logger
{
public:
  LoggerXml(const char *name):Logger(name) {}
  void printFunction(const std::string& funcName, ...);
};


#endif
