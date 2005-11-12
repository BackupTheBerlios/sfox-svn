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

  clipmap = new ClipMap(127);

//   ImageLoader::ImageData *imgData = mipmap->getLevel(1)->getImageData();
//   imgData->pixelFormat = PF_LUMINANCE;
// //   texture = g_TextureManager.create("mipm", PF_LUMINANCE, imgData->width,
// //                                     imgData->height);
//   texture = g_TextureManager.create("mipm", PF_LUMINANCE, 128, 128);
//   glPixelStorei(GL_UNPACK_SKIP_PIXELS,128);
//   glPixelStorei(GL_UNPACK_SKIP_ROWS, 128);
//   glPixelStorei(GL_UNPACK_ROW_LENGTH, imgData->width);
// //   texture->setData( imgData->data, imgData->pixelFormat, imgData->width,
// //   imgData->height );
//   texture->setData( imgData->data, 0, imgData->pixelFormat, 128,
//                      128);

// //   texture = g_TextureManager.load("test", DATAPATH"/media/clipmap/terrain/bigterrain.png");
//   //texture = g_TextureManager.load("test", "test.png");
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

//   TextureUnits::activeUnit( 0 );
//   glEnable( GL_TEXTURE_2D );
//   TextureUnits::setEnvMode( TEM_REPLACE );
//   texture->bind();
//   texture->setMinFilter(TF_LINEAR);
//   texture->setMagFilter(TF_LINEAR);

// //  glScalef(1, 0.25, 1);
//   //glEnable(GL_BLEND);
//   glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
//   glBegin( GL_QUADS );
//   glTexCoord2f(0., 1.);
//   glVertex3f(-1, 1, 0);
//   glTexCoord2f(0., 0.);
//   glVertex3f(-1, -1, 0);
//   glTexCoord2f(1., 0.);
//   glVertex3f(1, -1, 0);
//   glTexCoord2f(1., 1.);
//   glVertex3f(1, 1, 0);
//   glEnd();

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
  default:
    break;
  }
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
