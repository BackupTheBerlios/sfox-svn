#ifndef TESTAPP_H
#define TESTAPP_H

#include "framework/application.h"


namespace StarEngine {
  class FontGL;
  class Camera;
  class Texture;
  class Trackball;
  class FramebufferObject;
  class Shader;
}

class TestApp : public StarEngine::Application {
private:
  StarEngine::Trackball *trackball;
  StarEngine::FontGL *font;
  StarEngine::Camera *cam;
  StarEngine::Texture *texture;
  StarEngine::FramebufferObject *fbo;
  StarEngine::Shader *shaderPass1;
  StarEngine::Shader *shaderPass2;
  StarEngine::Shader *shaderPass3;
  StarEngine::Shader *shaderPass4;

  void printInfos();
  void drawCube(float sizex, float sizey, float sizez);
  void computeRays();
  void moveOnRay(float dt);

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
