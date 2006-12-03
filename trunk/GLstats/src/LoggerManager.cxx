#include "LoggerManager.h"

LoggerManager *LoggerManager::m_instance = NULL;

LoggerManager *
LoggerManager::getInstance()
{
  if(!m_instance)
    m_instance = new LoggerManager;
  return m_instance;
}
