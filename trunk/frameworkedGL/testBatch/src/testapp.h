#ifndef TESTAPP_H
#define TESTAPP_H

#include "framework/application.h"


namespace StarEngine {
  class FontGL;
  class Camera;
  class Texture;
  class Trackball;
  class GeometricBatch;
}

class TestApp : public StarEngine::Application {
private:
  StarEngine::Trackball *trackball;
  StarEngine::FontGL *font;
  StarEngine::Camera *cam;
  StarEngine::Texture *texture;
  StarEngine::GeometricBatch *geomBatch;
  StarEngine::GeometricBatch *geomBatch2;

  void printInfos();

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
  void mouseButtonUp(const SDL_MouseButtonEvent &m);
  void mouseMotion(const SDL_MouseMotionEvent &motion);
};

#endif
