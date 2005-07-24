#ifndef LIGHT_H
#define LIGHT_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

namespace StarEngine {
  class Light {
  public:
    Light(GLenum light);
    ~Light();

    void enable() {
      glEnable(lightId);
    }

    void setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1);
    void setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1);
    void setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1);
    void setPosition(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 1);
    void setAttenuation(GLfloat c, GLfloat l, GLfloat q);

  private:
    GLenum lightId;
  };
}
#endif
