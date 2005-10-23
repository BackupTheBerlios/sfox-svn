#include "testapp.h"
#include "GL/glew.h"

#define DATAPATH "../../common"

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

  font = new FontGL(DATAPATH"/media/fonts/vera.ttf", 16, 128);

  cam = new Camera(60, 0.001, 50);
  cam->setPos(0, 0, 4);

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

  IndicesBatch *ind = new IndicesBatch();
  ind->setIndices(4, SE_UNSIGNED_INT, indices, UT_STATIC_DRAW);

  geomBatch = new GeometricBatch();
  geomBatch->setVertexFormat("vertex:float3 color0:float3");
  geomBatch->setPrimitiveMode(PM_QUADS);
//   geomBatch->setVertices(sizeof(vertices), vertices, UT_STATIC_DRAW);
  geomBatch->setVertices(4*6*sizeof(float), NULL, UT_STATIC_DRAW);
  float *tmp = (float *)geomBatch->lock(AT_WRITE_ONLY);
  memcpy(tmp, vertices, sizeof(vertices));
  geomBatch->unlock();
  geomBatch->setIndicesBatch(ind);

  geomBatch2 = new GeometricBatch();
  geomBatch2->setVertexFormat("vertex:float3 color0:float3");
  geomBatch2->setPrimitiveMode(PM_QUADS);
  geomBatch2->setVertices(sizeof(vertices), vertices2, UT_STATIC_DRAW);
}


/*****************************************************************************/
/* Display                                                                   */
/*****************************************************************************/
void
TestApp::render() {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  cam->toOpenGL();
  trackball->toOpenGL();

  glColor3f(1,0,0);
  //geomBatch->drawArrays(0);
  geomBatch2->drawArrays(0);
  geomBatch->drawElements(4);

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
