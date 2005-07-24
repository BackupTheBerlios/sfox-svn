#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

#include "material.h"
#include "texture.h"

namespace StarEngine {
  Material::Material()
    : shininess(0), diffuse(.8f, .8f, .8f), specular(0, 0, 0),
      ambient(.2f, .2f, .2f), tex(0)
  {
  }

  Material::~Material()
  {
    delete tex;
    tex = 0;
  }

  void
  Material::toOpenGL()
  {
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &diffuse.r);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &specular.r);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &ambient.r);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  }
}
