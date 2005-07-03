#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>

#include "opengl/glext.h"
#include "math/quaternion.h"
#include "opengl/font.h"
#include "testapp.h"
#include "opengl/texture.h"
#include "opengl/camera.h"
#include "opengl/renderer.h"
#include "opengl/mesh.h"
#include "opengl/vertexbuffer.h"
#include "opengl/light.h"
#include "opengl/object3d.h"
#include "opengl/material.h"
#include "opengl/program.h"
#include "opengl/shader.h"

/*****************************************************************************/
/* Init&quit                                                                 */
/*****************************************************************************/
TestApp::TestApp()
  : Application(800, 600, "TestApp"), speedFactor(1.0f), mouseMode(MOUSEFREE)
{
  hideCursor(false);
  grabInput(false);
}

void
TestApp::quit()
{
  delete shader;
  delete obj;
  delete font;
  delete light;
  delete cam;
  FontGL::exit();
}

void
TestApp::init() {
  GLext::init();
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0, 0, 0, 0);
  
  Texture *texNormal = new Texture("data/normalmap1.png",
                                 Texture::TEXTURE_2D,
                                 Texture::RGB|Texture::BUILD_MIPMAPS|
                                 Texture::LINEAR_MIPMAP_LINEAR);
  Material *m = new Material();
  //m->setTexture(texNormal);
  m->setDiffuse(0.0f, 0.0f, 0.5f);
  m->setSpecular(1, 1, 1);
  m->setShininess(16);

  Mesh meshTest("data/monkey.scn");
  obj = new Object3d(&meshTest);
  obj->setMaterial(m);

  font = new FontGL("data/vera.ttf", 16, 128);

  cam = new Camera(60, 0.001, 50);
  cam->setPos(0, 0, 4);

  light = new Light(GL_LIGHT0);
  light->enable();
  light->setDiffuse(1, 1, 1, 1);
  
  shader = new Shader();
  shader->setVertexShader("shaders/pplight.vert");
  shader->setFragmentShader("shaders/pplight.frag");
  shader->link();
  shader->validate();
}


/*****************************************************************************/
/* Display                                                                   */
/*****************************************************************************/
void
TestApp::render() {
  if(getFPS()>1.0f)
    speedFactor = SPEEDCONST/getFPS();
  
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  cam->toOpenGL();
  light->setPosition(-2, 2, 3, 1);

  Renderer::printGLError();

  Material *mat = obj->getMaterial();
  //Texture *tex = mat->getTexture();
  obj->bind();
  //tex->bind();
  //tex->setTexEnv(GL_TEXTURE_ENV_MODE, GL_REPLACE);
  mat->toOpenGL();
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
//  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glPushMatrix();
  //glTranslatef(0, 0, -alpha/100.);
  //glRotatef(alpha, 0, 1, 0);
  alpha+=0.1;
  shader->bind();
  obj->render();
  glPopMatrix();

  Shader::useFixedPipeline();
  
  printInfos();
}

void
TestApp::printInfos()
{
  font->setColor(1, 1, 1);
  font->printf(0, height-16, "Fps: %.2f", getFPS());
  font->printf(0, height-32, "Speed factor: %.6f", speedFactor);
  
  font->setColor(1, 0, 0);
  font->printf(0, 16, "OpenGL error: %s", Renderer::getGLErrorString());
}

void
TestApp::idle() {
  doMove();  
}

/*****************************************************************************/
/* Keyboard Input                                                            */
/*****************************************************************************/
void
TestApp::keyDown(const SDL_keysym &key) {
  switch(key.sym) {
  case SDLK_ESCAPE:
    halt(0);
    break;
  default:
    break;
  }
}

void
TestApp::keyUp(const SDL_keysym &key)
{
}

void
TestApp::doMove()
{
  if(isDown(SDLK_UP))
    cam->moveAlongView(0.05*speedFactor);
  else if(isDown(SDLK_DOWN))
    cam->moveAlongView(-0.05*speedFactor);
  if(isDown(SDLK_LEFT))
    cam->sideMove(-0.05*speedFactor);
  else if(isDown(SDLK_RIGHT))
    cam->sideMove(0.05*speedFactor);
}

/*****************************************************************************/
/* Mouse Input                                                               */
/*****************************************************************************/
void
TestApp::mouseMotion(const SDL_MouseMotionEvent &motion)
{
  if(mouseMode==MOUSEFPS) {
    if(motion.x == width/2 && motion.y == height/2)
      return;
    cam->mouseMove((float)motion.xrel/10.0f, (float)motion.yrel/10.0f);
    warpMouse(width/2, height/2);
  }
}

void
TestApp::mouseButtonDown(const SDL_MouseButtonEvent &m)
{
  if(m.button==SDL_BUTTON_RIGHT)
    mouseMode = (mouseMode+1)%MOUSELAST;

  switch(mouseMode) {
  case MOUSEFPS:
    hideCursor(true);
    grabInput(true);
    warpMouse(width/2, height/2);
    break;
  case MOUSEFREE:
    grabInput(false);
    hideCursor(false);
    break;
  }
}
