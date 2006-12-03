#ifndef GLSTATCONFIG_H
#define GLSTATCONFIG_H

#include <vector>

class GLStatConfig {
public:
  /**
   * Set the number of minimum frames before computing FPS
   */
  void setFPSSamplingRate(size_t frame) { m_FPSSamplingRate = frame; }

  /**
   * Returns the number of minimum frames before computing FPS
   */
  size_t getFPSSamplingRate() { return m_FPSSamplingRate; }

  static GLStatConfig *getInstance();

private:
  size_t m_FPSSamplingRate;

  static GLStatConfig *m_instance;
};

#define glStatConfig GLStatConfig::getInstance()

#endif
