#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <GL/glu.h>

#include <SDL/SDL_syswm.h>

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
#include "opengl/shader.h"
#include "opengl/pbuffer.h"

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
  delete font;
  delete cam;
  FontGL::exit();
}

void
TestApp::init() {
  GLext::init();
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0, 0, 0, 0);
  
  font = new FontGL("data/vera.ttf", 16, 128);

  cam = new Camera(60, 0.001, 50);
  cam->setPos(0, 0, 4);

  shader = new Shader();
  /*  shader->setFragmentShader("shaders/calcnorm.frag");
  shader->link();
  shader->validate();*/

  progFp = new Shader(Shader::ARB);
  progFp->setFragmentShader("shaders/calcnorm.fp");

  tex = new Texture("data/nuage.png");
  printf("OpenGL error: %s\n", Renderer::getGLErrorString());
}


/*****************************************************************************/
/* Display                                                                   */
/*****************************************************************************/
void
TestApp::render() {
  if(getFPS()>1.0f)
    speedFactor = SPEEDCONST/getFPS();
  
  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  cam->toOpenGL();

  /*  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(800, 0, 600, 0, 0, 1.);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();*/

  glEnable(GL_FRAGMENT_PROGRAM_ARB);
  progFp->bind();
  //shader->setUniform("heightfield", 0);
  glClientActiveTextureARB(GL_TEXTURE0);
  tex->bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glColor3f(1, 1, 0);
  glBegin(GL_QUADS);
  for(int i = 0; i < 1; i++) {
    glTexCoord2f(0, 1);
    glVertex3f(-0.01, 0.01, i*0.01);
    glTexCoord2f(0, 0);
    glVertex3f(-0.01, -0.01, i*0.01);
    glTexCoord2f(1, 0);
    glVertex3f(0.01, -.01, i*0.01);
    glTexCoord2f(1, 1);
    glVertex3f(.01, .01, i*0.01);
  }
  glEnd();
/*  glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex2f(0, 0);
    glTexCoord2f(0, 0);
    glVertex2f(256, 0);
    glTexCoord2f(1, 0);
    glVertex2f(256, 256);
    glTexCoord2f(1, 1);
    glVertex2f(0, 256);
  glEnd();
*/
  glDisable(GL_FRAGMENT_PROGRAM_ARB);
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
  if(isDown(SDLK_UP)||isDown(SDLK_z))
    cam->moveAlongView(0.05*speedFactor);
  else if(isDown(SDLK_DOWN)||isDown(SDLK_s))
    cam->moveAlongView(-0.05*speedFactor);
  if(isDown(SDLK_LEFT)||isDown(SDLK_q))
    cam->sideMove(-0.05*speedFactor);
  else if(isDown(SDLK_RIGHT)||isDown(SDLK_d))
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
