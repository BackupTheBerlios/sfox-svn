#ifndef RENDERER_H
#define RENDERER_H

class Renderer {
public:
  static void printModelView();
  static bool printGLError();

  static const char * getGLErrorString();
};

#endif
