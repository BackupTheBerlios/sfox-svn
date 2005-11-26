#ifndef EFFECTCG_H
#define EFFECTCG_H
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include "ressourcemanager.h"

namespace StarEngine {
  class Texture;
};

namespace StarEngine {
  class EffectCG : public Ressource {
  public:
    enum ProfileType { VERTEX, FRAGMENT };

  private:
    CGeffect effect;
    CGprofile profileCG;
    CGcontext context;

    CGGLenum getProfileCGGL( ProfileType profile );

  public:
    EffectCG();
    ~EffectCG();

    static void init();
    static void quit();

    void loadSourceFromFile(const char *filename,
                            const char **args = NULL);
    CGtechnique getAndValidateTechnique(const char *name);

    void enableTextureParameter(const char *name);
    void disableTextureParameter(const char *name);
    static void setManageTextureParameters(bool enable);

    void setSamplerState(const char *name, Texture *tex);
    void setSamplerState(CGparameter p);
    void setupSampler(const char *name, Texture *tex);

    CGprogram createProgramFromEffect(CGparameter p, char *progName,
                                      const char** args);
    void evaluateProgram(CGprogram prog, float *obuf, int ncomp,
                         int nx, int ny, int nz);


    CGpass getFirstPass(CGtechnique technique);
    CGpass getNextPass(CGpass pass);
    void setPassState(CGpass pass);
    void resetPassState(CGpass pass);

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
