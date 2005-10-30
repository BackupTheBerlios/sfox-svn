#ifndef TESTAPP_H
#define TESTAPP_H

#include "framework/application.h"

class FontGL;
class Camera;
class Light;
class Object3d;
class Shader;
class PBuffer;
class Texture;

class TestApp : public Application {
private:
  FontGL *font;
  Camera *cam;
  Shader *shader;
  Shader *progFp;
  Texture *tex;

  float alpha;

  static const int SPEEDCONST = 60;
  float speedFactor;
  void doMove();

  enum { MOUSEFPS = 0, MOUSEFREE, MOUSELAST };
  unsigned int mouseMode;
  
  void printInfos();

public:
  TestApp();
  ~TestApp() {}
  
  void init();
  void quit();
  void render();
  void idle();

  void mouseButtonDown(const SDL_MouseButtonEvent &m);
  void keyDown(const SDL_keysym &key);
  void keyUp(const SDL_keysym &key);
  void mouseMotion(const SDL_MouseMotionEvent &motion);
};

#endif
