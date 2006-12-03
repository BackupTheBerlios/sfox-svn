#include "GLStatConfig.h"

GLStatConfig *GLStatConfig::m_instance = NULL;

GLStatConfig *
GLStatConfig::getInstance()
{
  if(!m_instance)
    m_instance = new GLStatConfig;
  return m_instance;
}
