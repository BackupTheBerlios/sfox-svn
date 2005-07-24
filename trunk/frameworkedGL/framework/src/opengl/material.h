#ifndef MATERIAL_H
#define MATERIAL_H

namespace StarEngine {
  class Texture;

  class RGBA {
  public:
    float r,g,b,a;
    RGBA(float r, float g, float b, float a=1.0f):r(r),g(g),b(b),a(a) {}
    void setValues(float r, float g, float b, float a=1.0f) {
      this->r = r; this->g=g;
      this->b = b; this->a = a;
    }
  };

  class Material {
  private:
    float shininess;
    RGBA diffuse, specular, ambient;
    Texture *tex; //Destructor deletes texture

  public:
    Material();
    ~Material();
    void toOpenGL();

    Texture *getTexture() { return tex; }
    void setTexture(Texture *t) { tex = t; }

    //Colors setters
    void setDiffuse(float r, float g, float b, float a = 1) {
      diffuse.setValues(r, g, b, a);
    }

    void setSpecular(float r, float g, float b, float a = 1) {
      specular.setValues(r, g, b, a);
    }

    void setAmbient(float r, float g, float b, float a = 1) {
      ambient.setValues(r, g, b, a);
    }

    void setShininess(float s) {
      shininess = s;
    }
  };
}
#endif
