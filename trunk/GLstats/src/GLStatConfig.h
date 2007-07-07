#ifndef GLSTATCONFIG_H
#define GLSTATCONFIG_H

#include <vector>

#include "Singleton.h"

class GLStatConfig : public Singleton<GLStatConfig>
{
public:
  /**
   * Set the number of minimum millisecondes before computing FPS
   */
  float setFPSSamplingRate(float ms) { m_FPSSamplingRate = ms; }

  /**
   * Returns the number of minimum millisecondes before computing FPS
   */
  float getFPSSamplingRate() { return m_FPSSamplingRate; }

private:
  float m_FPSSamplingRate;
};

#define glStatConfig GLStatConfig::getInstance()

#endif
