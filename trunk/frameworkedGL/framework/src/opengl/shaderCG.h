#ifndef SHADER_H
#define SHADER_H
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

namespace StarEngine {
  class ShaderCG {
  public:
    enum ProfileType { VERTEX, FRAGMENT };

  private:
    static CGcontext context;
    CGprogram program;

    CGGLenum getProfileCGGL( ProfileType profile );

  public:
    ShaderCG();
    ~ShaderCG();

    static void init();
    static void quit();

    void loadSourceFromFile(const char *filename, ProfileType profile,
                            const char *entry = NULL,
                            const char **args = NULL);

  };
}
#endif
