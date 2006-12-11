#ifndef GLSTATCONFIG_H
#define GLSTATCONFIG_H

#include <vector>

class GLStatConfig {
public:
  /**
   * Set the number of minimum millisecondes before computing FPS
   */
  float setFPSSamplingRate(float ms) { m_FPSSamplingRate = ms; }

  /**
   * Returns the number of minimum millisecondes before computing FPS
   */
  float getFPSSamplingRate() { return m_FPSSamplingRate; }

  static GLStatConfig *getInstance();

private:
  float m_FPSSamplingRate;

  static GLStatConfig *m_instance;
};

#define glStatConfig GLStatConfig::getInstance()

#endif
