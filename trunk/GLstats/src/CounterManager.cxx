#include "CounterManager.h"

CounterManager *CounterManager::m_instance = 0;

CounterManager *
CounterManager::getInstance()
{
  if(!m_instance)
    m_instance = new CounterManager;
  return m_instance;
}
