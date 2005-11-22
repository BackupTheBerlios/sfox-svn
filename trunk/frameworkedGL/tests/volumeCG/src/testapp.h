#ifndef TESTAPP_H
#define TESTAPP_H

#include "starengine.h"

namespace StarEngine {
  class FontGL;
  class Camera;
  class Texture;
  class Trackball;
  class FramebufferObject;
  class ShaderCG;
  class Image;
}

class TestApp : public StarEngine::Application {
private:
  StarEngine::Trackball *trackball;
  StarEngine::FontGL *font;
  StarEngine::Camera *cam;
  StarEngine::Texture *texture;
  StarEngine::FramebufferObject *fbo;
  StarEngine::ShaderCG *shaderVertPass1;
  StarEngine::ShaderCG *shaderFragPass1;
  StarEngine::ShaderCG *shaderFragPass2;
  StarEngine::ShaderCG *shaderFragPass3;
  StarEngine::ShaderCG *shaderFragPass4;

  static const int cubeX = 2;
  static const int cubeY = 2;
  static const int cubeZ = 2;

  void printInfos();
  void drawCube(float sizex, float sizey, float sizez);
  void computeRays();
  void moveOnRay(float dt);

  unsigned char *genPreIntegrated(StarEngine::Image *img);

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
