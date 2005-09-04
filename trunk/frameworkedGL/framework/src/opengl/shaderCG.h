#ifndef SHADERCG_H
#define SHADERCG_H
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

namespace StarEngine {
  class Texture;
};

namespace StarEngine {
  class ShaderCG {
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
                            const char *entry = NULL,
                            const char **args = NULL);
    void bind();
    void unbind();

    CGparameter getNamedParameter(const char *name);
    void setGLMVPMatrix( const char *name );

    void setTextureParameter(const char *name, Texture *tex);
    void enableTextureParameter(const char *name);
    void disableTextureParameter(const char *name);
  };
}
#endif
