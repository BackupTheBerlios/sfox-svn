#ifdef WIN32
#include <windows.h>
#endif

#include <cassert>

#include "light.h"

namespace StarEngine {
  Light::Light(GLenum light)
    :lightId(light)
  {
    assert(light>=GL_LIGHT0&&light<=GL_LIGHT7);
  }

  Light::~Light()
  {
    glDisable(lightId);
  }

  void
  Light::setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
  {
    GLfloat tmp[4] = {r, g, b, a};
    glLightfv(lightId, GL_AMBIENT, tmp);
  }

  void
  Light::setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
  {
    GLfloat tmp[4] = {r, g, b, a};
    glLightfv(lightId, GL_DIFFUSE, tmp);
  }

  void
  Light::setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
  {
    GLfloat tmp[4] = {r, g, b, a};
    glLightfv(lightId, GL_SPECULAR, tmp);
  }

  void
  Light::setPosition(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
  {
    GLfloat tmp[4] = {x, y, z, w};
    glLightfv(lightId, GL_POSITION, tmp);
  }

  void
  Light::setAttenuation(GLfloat c, GLfloat l, GLfloat q)
  {
    glLightf(lightId, GL_CONSTANT_ATTENUATION, c);
    glLightf(lightId, GL_LINEAR_ATTENUATION, l);
    glLightf(lightId, GL_QUADRATIC_ATTENUATION, q);
  }
}
