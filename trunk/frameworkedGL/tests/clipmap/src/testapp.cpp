#include "testapp.h"
#include "GL/glew.h"

#include "clipmap.h"

#define DATAPATH "../../common"

using namespace StarEngine;

/*****************************************************************************/
/* Init&quit                                                                 */
/*****************************************************************************/
TestApp::TestApp()
  : Application(800, 600, "TestApp"), mouseMode(MOUSEFREE)
{
  hideCursor(false);
  grabInput(false);
  speedFactor = 0.1;
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

  font = new FontGL(DATAPATH"/media/fonts/vera.ttf", 16, 128);

  cam = new Camera(60, 0.001, 50);
  cam->setPos(0, 1, 0);

  trackball = new Trackball(width,  height);

  static float vertices[] = {
    -1, 1, 0,
    1,0,0,
    -1, -1, 0,
    1,1,0,
    1, -1, 0,
    1,0,0,
    1, 1, 0,
    1,1,0
  };

  static float vertices2[] = {
    0, 2, 0,
    0,1,0,
    0, 0, 0,
    0,1,0,
    2, 0, 0,
    0,1,0,
    2, 2, 0,
    0,1,0
  };

  static unsigned int indices[] = {
    0, 1, 2, 3
  };

  indBatch = new IndicesBatch();
  indBatch->setIndices(4, SE_UNSIGNED_INT, indices, UT_STATIC_DRAW);

  geomBatch = new GeometricBatch();
  geomBatch->setVertexFormat("vertex:float3 color0:float3");
  geomBatch->setPrimitiveMode(PM_QUADS);
//   geomBatch->setVertices(sizeof(vertices), vertices, UT_STATIC_DRAW);
  geomBatch->setVertices(4*6*sizeof(float), NULL, UT_STATIC_DRAW);
  float *tmp = (float *)geomBatch->lock(AT_WRITE_ONLY);
  memcpy(tmp, vertices, sizeof(vertices));
  geomBatch->unlock();
  //geomBatch->setIndicesBatch(ind);

  geomBatch2 = new GeometricBatch();
  geomBatch2->setVertexFormat("vertex:float3 color0:float3");
  geomBatch2->setPrimitiveMode(PM_QUADS);
  geomBatch2->setVertices(sizeof(vertices), vertices2, UT_STATIC_DRAW);

  clipmap = new ClipMap(255);
}


/*****************************************************************************/
/* Display                                                                   */
/*****************************************************************************/
void
TestApp::render() {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  cam->toOpenGL();
//  trackball->toOpenGL();

  glColor3f(1,0,0);

  //geomBatch2->bind();
  //geomBatch2->drawArrays(0);

  //geomBatch->bind();
  //indBatch->bind();
//  geomBatch->drawElements(indBatch, 4);
//  indBatch->unbind();
//  geomBatch->unbind();

  clipmap->render();

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
}

void
TestApp::doMove()
{
  if(isDown(SDLK_z))
    cam->moveAlongView(0.05*speedFactor);
  else if(isDown(SDLK_s))
    cam->moveAlongView(-0.05*speedFactor);
  if(isDown(SDLK_q))
    cam->sideMove(-0.05*speedFactor);
  else if(isDown(SDLK_d))
    cam->sideMove(0.05*speedFactor);
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
