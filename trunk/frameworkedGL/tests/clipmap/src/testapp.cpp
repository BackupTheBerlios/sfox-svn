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
  levelToDisplay = 0;
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
  glClearColor(0, 1, 1, 0);

  font = new FontGL(DATAPATH"/media/fonts/vera.ttf", 16, 128);

  cam = new Camera(60, 0.001, 50);
  cam->setPos(0, 1, 0);

  trackball = new Trackball(width,  height);

  clipmap = new ClipMap(255);

  //Load float terrain texture
  fprintf(stderr, "Generating mipmap...");
  mipmap = new Mipmap;
  mipmap->buildMipmap(DATAPATH"/media/clipmap/terrain/bigterrain.png", 4);
  fprintf(stderr, "Done\n");

  mipmap->getTextures(levels, 1024, 1024, 256, 256);

//  Texture2D *tex = (Texture2D *)g_TextureManager.load("grass", DATAPATH"/media/clipmap/textures/grass.png");
  //levels.push_back(tex);

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

  TextureUnits::activeUnit( 0 );
  glEnable( GL_TEXTURE_2D );
  TextureUnits::setEnvMode( TEM_REPLACE );
  levels[levelToDisplay]->bind();
  levels[levelToDisplay]->setMinFilter(TF_NEAREST);
  levels[levelToDisplay]->setMagFilter(TF_NEAREST);
  levels[levelToDisplay]->setWrapS( TW_CLAMP_TO_EDGE );
  levels[levelToDisplay]->setWrapT( TW_CLAMP_TO_EDGE );

//  glScalef(1, 0.25, 1);
  //glEnable(GL_BLEND);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width-1, height-1, 0, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(width-100, 0, 0);

  glBegin( GL_QUADS );
  glTexCoord2f(0., 0.);
  glVertex3f(0, 0, 0);
  glTexCoord2f(0., 1.);
  glVertex3f(0, 100, 0);
  glTexCoord2f(1., 1.);
  glVertex3f(100, 100, 0);
  glTexCoord2f(1., 0.);
  glVertex3f(100, 0, 0);
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
    levelToDisplay = (levelToDisplay+1)%levels.size();
    break;
  case SDLK_k:
    levelToDisplay = (levelToDisplay+levels.size()-1)%levels.size();
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
