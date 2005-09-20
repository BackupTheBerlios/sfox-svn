#ifndef SHADERCG_H
#define SHADERCG_H
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include "ressourcemanager.h"

namespace StarEngine {
  class Texture;
};

namespace StarEngine {
  class ShaderCG : public Ressource {
  public:
    enum ProfileType { VERTEX, FRAGMENT };

  private:
    static CGcontext context;
    CGprogram program;
    CGprofile profileCG;

    CGGLenum getProfileCGGL( ProfileType profile );

  public:
    ShaderCG();
    ~ShaderCG();

    static void init();
    static void quit();

    void loadSourceFromFile(const char *filename, ProfileType profile,
                            const CGenum program_type = CG_SOURCE,
                            const char *entry = NULL,
                            const char **args = NULL);
    void bind();
    void unbind();


    void enableTextureParameter(const char *name);
    void disableTextureParameter(const char *name);

    /**
     Set the uniform name with the current model view projection matrix.
     @param name is the name of the uniform
    */
    void setGLMVPMatrix( const char *name );

/*****************************************************************************/
/* Uniforms setters                                                          */
/*****************************************************************************/
    /**
     Return the handle to uniform name
     @param name is the name of the uniform
    */
    CGparameter getNamedParameter(const char *name);
    void setTextureParameter(const char *name, Texture *tex);

    void setParameter1f(const char *name, float x);
    void setParameter2f(const char *name, float x, float y);
    void setParameter3f(const char *name, float x, float y, float z);
    void setParameter4f(const char *name, float x, float y, float z,
                        float w);
    void setParameter1fv(const char *name, const float *v);
    void setParameter2fv(const char *name, const float *v);
    void setParameter3fv(const char *name, const float *v);
    void setParameter4fv(const char *name, const float *v);
    void setParameter1d(const char *name,  double x);
    void setParameter2d(const char *name,  double x, double y);
    void setParameter3d(const char *name,  double x, double y, double z);
    void setParameter4d(const char *name,  double x, double y, double z,
                        double w);
    void setParameter1dv(const char *name, const double *v);
    void setParameter2dv(const char *name, const double *v);
    void setParameter3dv(const char *name, const double *v);
    void setParameter4dv(const char *name, const double *v);
  };
}
#endif
