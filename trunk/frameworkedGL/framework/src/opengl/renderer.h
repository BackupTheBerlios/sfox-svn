#ifndef RENDERER_H
#define RENDERER_H

#define printGLError() printGLError2( __FILE__, __LINE__ )

namespace StarEngine {
  class Renderer {
  public:
    static void printModelView();
    ///static bool printGLError();
    static bool printGLError2(const char *i, int l);

    static const char * getGLErrorString();
  };
}
#endif
