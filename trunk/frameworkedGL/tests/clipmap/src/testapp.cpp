#include "testapp.h"
#include "GL/glew.h"

#include "clipmap.h"
#include "mipmap.h"

#define DATAPATH "../../common"

using namespace StarEngine;

/*****************************************************************************/
/* Init&quit                                                                 */
/*****************************************************************************/
TestApp::TestApp()
  : Application(800, 600, "TestApp"), mouseMode(MOUSEFREE)
{
  hideCursor(false);
  grabInput(true);
  grabInput(false);
  speedFactor = 0.1f;
  levelToDisplay = 0;
}

void
TestApp::quit()
{
  delete clipmap;
  delete font;
  FontGL::exit();
}

void
TestApp::init() {
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0, 0, 0, 0);

  font = new FontGL(DATAPATH"/media/fonts/vera.ttf", 16, 128);

  cam = new Camera(60, 0.001f, 50);
  cam->setPos(0, 1, 0);

  trackball = new Trackball(width,  height);

  numLevels = 4;
  clipmap = new ClipMap(255, numLevels);
}


/*****************************************************************************/
/* Display                                                                   */
/*****************************************************************************/
void
TestApp::render() {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  cam->toOpenGL();

  glColor3f(1,0,0);


  glPushMatrix();
  clipmap->render();
  glPopMatrix();

  clipmap->drawDebugMipmap(levelToDisplay);

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
  font->printf(0, 32, "CG error: %s", Renderer::getCGErrorString());
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
  switch(key.sym) {
  case SDLK_w:
    clipmap->setWireframe(!clipmap->isWireframe());
    break;
  case SDLK_l:
    levelToDisplay = (levelToDisplay+1)%numLevels;
    break;
  case SDLK_k:
    levelToDisplay = (levelToDisplay+numLevels-1)%numLevels;
    break;
  default:
    break;
  }
}

void
TestApp::doMove()
{
  if(isDown(SDLK_z))
    cam->moveAlongView(0.5*speedFactor);
  else if(isDown(SDLK_s))
    cam->moveAlongView(-0.5*speedFactor);
  if(isDown(SDLK_q))
    cam->sideMove(-0.5*speedFactor);
  else if(isDown(SDLK_d))
    cam->sideMove(0.5*speedFactor);
}

/*****************************************************************************/
/* Mouse    Input                                                            */
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
