#ifndef TESTAPP_H
#define TESTAPP_H

#include "starengine.h"

class ClipMap;
class Mipmap;

namespace StarEngine {
  class FontGL;
  class Camera;
  class Texture;
  class Trackball;
  class GeometricBatch;
  class IndicesBatch;
}

class TestApp : public StarEngine::Application {
private:
  StarEngine::Trackball *trackball;
  StarEngine::FontGL *font;
  StarEngine::Camera *cam;
  StarEngine::Texture *texture;

  ClipMap *clipmap;

  void printInfos();

  enum {MOUSEFPS=0, MOUSEFREE, MOUSELAST};
  unsigned int mouseMode;

  static const int SPEEDCONST = 60;
  float speedFactor;
  void doMove();

public:
  TestApp();
  ~TestApp() {}

  void init();
  void quit();
  void render();
  void idle();

  void keyDown(const SDL_keysym &key);
  void keyUp(const SDL_keysym &key);

  void mouseButtonDown(const SDL_MouseButtonEvent &m);
  void mouseMotion(const SDL_MouseMotionEvent &motion);
};

#endif
