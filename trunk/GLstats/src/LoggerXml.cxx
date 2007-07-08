#include "LoggerXml.h"

#include <iostream>
#include <cstdarg>
#include <cassert>

#include <GL/gl.h>

void
LoggerXml::printFunction(const std::string& funcName, ...)
{
//   std::string comma;
//   std::string paramList;
//   va_list vl;
//   va_start(vl, funcName);
//   GLInfoFuncs::ParamList pl = glInfoFuns->getParamsTypeList(funcName);
//   for(i = 0; i < pl.size(); i++)
//   {
//     switch(pl[i])
//     {
//     case LG_ENUM:
//       GLenum e = va_arg(vl, GLenum);
//       paramString << e;
//       break;
//     default:
//       assert(0);
//     }
//     paramString << comma;
//     comma = ", ";
//   }

//     std::cerr << funcName << "( " << paramString << " )" << std::endl;
//   va_end(vl);
  std::cerr << funcName << std::endl;
}
