#ifdef _WIN32
#include <windows.h>
#endif

#include "math/quaternion.h"
#include "opengl/font.h"
#include "opengl/image.h"
#include "testapp.h"
#include "opengl/texture.h"
#include "opengl/texturemanager.h"
#include "opengl/textureunit.h"
#include "opengl/texture2d.h"
#include "opengl/camera.h"
#include "opengl/renderer.h"
#include "opengl/mesh.h"
#include "opengl/vertexbuffer.h"
#include "opengl/light.h"
#include "opengl/object3d.h"
#include "opengl/material.h"
#include "opengl/shader.h"
#include "opengl/trackball.h"

using namespace StarEngine;

/*****************************************************************************/
/* Init&quit                                                                 */
/*****************************************************************************/
TestApp::TestApp()
  : Application(800, 600, "TestApp")
{
  hideCursor(false);
  grabInput(false);
}

void
TestApp::quit()
{
  delete font;
  FontGL::exit();
}

void
TestApp::init() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0, 0, 0, 0);

  font = new FontGL("data/vera.ttf", 16, 128);

  cam = new Camera(60, 0.001, 50);
  cam->setPos(0, 0, 4);

  trackball = new Trackball(width,  height);


  texture = g_TextureManager.load("test", "data/test.png");
}


/*****************************************************************************/
/* Display                                                                   */
/*****************************************************************************/
void
TestApp::render() {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  cam->toOpenGL();
  trackball->toOpenGL();

  TextureUnits::activeUnit( 0 );
  glEnable( GL_TEXTURE_2D );
  TextureUnits::setEnvMode( TEM_REPLACE );
  g_TextureManager.getByName( "test" )->bind();

  glColor3f(1, 1, 0);

  glBegin( GL_QUADS );

  glTexCoord2f(0., 1.);
  glVertex3f(-1, 1, 0);

  glTexCoord2f(0., 0.);
  glVertex3f(-1, -1, 0);

  glTexCoord2f(1., 0.);
  glVertex3f(1, -1, 0);

  glTexCoord2f(1., 1.);
  glVertex3f(1, 1, 0);
  glEnd();

  Renderer::printGLError();

  printInfos();
}

void
TestApp::printInfos()
{
  font->setColor(1, 1, 1);
  font->printf(0, height-16, "Fps: %.2f", getFPS());

  font->setColor(1, 0, 0);
  font->printf(0, 16, "OpenGL error: %s", Renderer::getGLErrorString());
}

void
TestApp::idle() {
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

/*****************************************************************************/
/* Mouse    Input                                                            */
/*****************************************************************************/
void
TestApp::mouseMotion(const SDL_MouseMotionEvent &motion)
{
  trackball->computeOrientation(motion.x, motion.y);
}

void
TestApp::mouseButtonDown(const SDL_MouseButtonEvent &m)
{
  if(m.button==SDL_BUTTON_LEFT)
    trackball->startRotation( m.x,  m.y );
}

void
TestApp::mouseButtonUp(const SDL_MouseButtonEvent &m)
{
  if(m.button==SDL_BUTTON_LEFT)
    trackball->stopRotation();
}
