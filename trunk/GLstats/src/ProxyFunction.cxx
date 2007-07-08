#include "ProxyFunction.h"

#include "GLFuncs.h"
#include "LoggerXml.h"
#include "LoggerManager.h"

#include <iostream>

#ifndef GLAPI
#define GLAPI __attribute__((visibility("default")))
#endif

#define PROXYGLFUNC(funcName, ret, argsDef, args, ...)          \
  GLAPI ret funcName argsDef                                    \
  {                                                             \
    LoggerXml* logXml;                                          \
    logXml = (LoggerXml*)loggerManager.getLogger("FuncLogger"); \
    logXml->printFunction(#funcName, ##__VA_ARGS__);              \
    GLFuncs<ret>::getInstance()->getFunction(#funcName) args;  \
  }

#include "generated/proxyglfunc.h"
