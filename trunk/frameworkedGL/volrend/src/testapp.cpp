#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>

#include "opengl/glext.h"
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
#include "opengl/fbo.h"

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
/****************************************************************************/

void
TestApp::quit()
{
  delete font;
  FontGL::exit();
}
/****************************************************************************/

void
TestApp::init() {
  GLext::init();
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0, 0, 0, 0);

  font = new FontGL("data/vera.ttf", 16, 128);

  cam = new Camera(60.f, 0.001f, 50.f);
  cam->setPos(0, 0, 4);

  trackball = new Trackball(width, height);

  texture = g_TextureManager.load("test", "data/test.png");
  Texture *texture = g_TextureManager.load("volData", "data/bonsai.dat");

  fbo = new FramebufferObject;
  fbo->bind();
  g_TextureManager.create("rttTmp", PF_RGBA16F, width, height);
  g_TextureManager.create("rttRays", PF_RGBA16F, width, height);
  g_TextureManager.create("rttRes", PF_RGBA16F, width, height);
  g_TextureManager.create("rttColor", PF_RGBA16F, width, height);
  Renderer::printGLError();
  g_TextureManager.create("rttDepth", PF_DEPTH24, width, height);
  fbo->attachTexture( g_TextureManager.getByName( "rttColor" ),
                      FramebufferObject::COLOR_ATTACHMENT0, 0 );
  Renderer::printGLError();
  fbo->attachTexture( g_TextureManager.getByName( "rttDepth" ),
                      FramebufferObject::DEPTH_ATTACHMENT, 0 );
  fbo->checkStatus();
  Renderer::printGLError();

  shaderPass1 = new Shader();
  shaderPass1->setVertexShader("shaders/pass1.vert");
  shaderPass1->setFragmentShader("shaders/pass1.frag");
  shaderPass1->link();
  shaderPass1->validate();

  shaderPass2 = new Shader();
  shaderPass2->setVertexShader("shaders/pass2.vert");
  shaderPass2->setFragmentShader("shaders/pass2.frag");
  shaderPass2->link();
  shaderPass2->validate();

  shaderPass2->bind();
  shaderPass2->setUniform("frontFace", 0);
  shaderPass2->setUniform("winScale", 1./width,  1./height);

  shaderPass3 = new Shader();
  shaderPass3->setVertexShader("shaders/pass1.vert");
  shaderPass3->setFragmentShader("shaders/pass3.frag");
  shaderPass3->link();
  shaderPass3->validate();

  //Set texture units
  shaderPass3->bind();
  shaderPass3->setUniform("volData", 0);
  shaderPass3->setUniform("raysDir", 1);
  //shaderPass3->setUniform("resTex", 2);
  shaderPass3->setUniform("winScale", 1./width,  1./height);
  shaderPass3->setUniform("t", 0.f);

  shaderPass4 = new Shader();
  shaderPass4->setVertexShader("shaders/pass1.vert");
  shaderPass4->setFragmentShader("shaders/pass4.frag");
  shaderPass4->link();
  shaderPass4->validate();

  shaderPass4->bind();
  shaderPass4->setUniform("resTex", 2);
  shaderPass4->setUniform("winScale", 1./width,  1./height);
}


/*****************************************************************************/
/* Display                                                                   */
/*****************************************************************************/
void
TestApp::drawCube(float sizex, float sizey, float sizez)
{
  sizex *= 0.5;
  sizey *= 0.5;
  sizez *= 0.5;

  glEnable(GL_CULL_FACE);

  glBegin( GL_QUADS );
   //Front face
   glTexCoord3f(0, 1., 1.);
   glVertex3f(-sizex, sizey, sizez);
   glTexCoord3f(0, 0., 1.);
   glVertex3f(-sizex, -sizey, sizez);
   glTexCoord3f(1, 0., 1.);
   glVertex3f(sizex, -sizey, sizez);
   glTexCoord3f(1, 1., 1.);
   glVertex3f(sizex, sizey, sizez);

   //Back face
   glTexCoord3f(0, 1., 0.);
   glVertex3f(-sizex, sizey, -sizez);
   glTexCoord3f(1, 1., 0.);
   glVertex3f(sizex, sizey, -sizez);
   glTexCoord3f(1, 0., 0.);
   glVertex3f(sizex, -sizey, -sizez);
   glTexCoord3f(0, 0., 0.);
   glVertex3f(-sizex, -sizey, -sizez);

   //Left face
   glTexCoord3f(0, 1., 0.);
   glVertex3f(-sizex, sizey, -sizez);
   glTexCoord3f(0, 0., 0.);
   glVertex3f(-sizex, -sizey, -sizez);
   glTexCoord3f(0, 0., 1.);
   glVertex3f(-sizex, -sizey, sizez);
   glTexCoord3f(0, 1., 1.);
   glVertex3f(-sizex, sizey, sizez);

   //Right face
   glTexCoord3f(1, 1., 0.);
   glVertex3f(sizex, sizey, -sizez);
   glTexCoord3f(1, 1., 1.);
   glVertex3f(sizex, sizey, sizez);
   glTexCoord3f(1, 0., 1.);
   glVertex3f(sizex, -sizey, sizez);
   glTexCoord3f(1, 0., 0.);
   glVertex3f(sizex, -sizey, -sizez);

   //Top face
   glTexCoord3f(1, 1., 0.);
   glVertex3f(sizex, sizey, -sizez);
   glTexCoord3f(0, 1., 0.);
   glVertex3f(-sizex, sizey, -sizez);
   glTexCoord3f(0, 1., 1.);
   glVertex3f(-sizex, sizey, sizez);
   glTexCoord3f(1, 1., 1.);
   glVertex3f(sizex, sizey, sizez);

   //Bottomop face
   glTexCoord3f(1, 0., 0.);
   glVertex3f(sizex, -sizey, -sizez);
   glTexCoord3f(1, 0., 1.);
   glVertex3f(sizex, -sizey, sizez);
   glTexCoord3f(0, 0., 1.);
   glVertex3f(-sizex, -sizey, sizez);
   glTexCoord3f(0, 0., 0.);
   glVertex3f(-sizex, -sizey, -sizez);

   glEnd();

}
/****************************************************************************/

void
TestApp::computeRays()
{
  fbo->bind();
  TextureUnits::activeUnit( 0 );

  Texture * texRays = g_TextureManager.getByName("rttRays");
  Texture * texTmp = g_TextureManager.getByName("rttTmp");
  texTmp->bind();
  texTmp->setMinFilter(TF_NEAREST);
  texTmp->setMagFilter(TF_NEAREST);

  texRays->bind();
  texRays->setMinFilter(TF_NEAREST);
  texRays->setMagFilter(TF_NEAREST);

  fbo->attachTexture( texTmp, FramebufferObject::COLOR_ATTACHMENT0, 0 );
  fbo->detachTexture( g_TextureManager.getByName( "rttDepth" ),
                      FramebufferObject::DEPTH_ATTACHMENT, 0 );
  fbo->checkStatus();

  // Render front faces
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  shaderPass1->bind();
  drawCube(cubeX, cubeY, cubeZ);
  glFlush();

  // Render back faces and compute rays
  fbo->attachTexture( texRays, FramebufferObject::COLOR_ATTACHMENT0, 0 );
  fbo->checkStatus();
  glClear(GL_COLOR_BUFFER_BIT);
  texTmp->bind();
  glCullFace(GL_FRONT);
  shaderPass2->bind();
  drawCube(cubeX, cubeY, cubeZ);

  Shader::useFixedPipeline();
  FramebufferObject::unbind();
}
/****************************************************************************/

void
TestApp::moveOnRay(float dt)
{
  fbo->bind();
  Texture * texRays = g_TextureManager.getByName("rttRays");
  Texture * texRes = g_TextureManager.getByName("rttRes");
  Texture * texVolData = g_TextureManager.getByName("volData");

  TextureUnits::activeUnit( 2 );
  texRes->bind();
  texRays->setMinFilter( TF_NEAREST );
  texRays->setMagFilter( TF_NEAREST );
  TextureUnits::activeUnit( 1 );
  texRays->bind();
  texRays->setMinFilter( TF_NEAREST );
  texRays->setMagFilter( TF_NEAREST );
  TextureUnits::activeUnit( 0 );
  texVolData->bind();
  texRays->setMinFilter( TF_LINEAR );
  texRays->setMagFilter( TF_LINEAR );
  texVolData->setWrapS( TW_CLAMP_TO_EDGE );
  texVolData->setWrapT( TW_CLAMP_TO_EDGE );
  texVolData->setWrapR( TW_CLAMP_TO_EDGE );

  fbo->attachTexture( texRes, FramebufferObject::COLOR_ATTACHMENT0, 0 );
  fbo->checkStatus();
  glClear(GL_COLOR_BUFFER_BIT);

  shaderPass3->bind();
  shaderPass3->setUniform("dt", 0.01f);
  float t = 0.0f;

  glCullFace( GL_BACK );
  shaderPass3->bind();
  shaderPass3->setUniform("t", t);
  drawCube(cubeX, cubeY, cubeZ);

  Shader::useFixedPipeline();
  FramebufferObject::unbind();
}
/****************************************************************************/

void
TestApp::render() {
  fbo->bind();
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  cam->toOpenGL();
  trackball->toOpenGL();

  computeRays();
  glFlush();
  moveOnRay( 0.01f );

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glDisable( GL_CULL_FACE );
  glDisable( GL_DEPTH_TEST );

  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, width-1, 0, height-1, 0, 10.);

  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glLoadIdentity();

  glEnable( GL_TEXTURE_2D );
  TextureUnits::setEnvMode( TEM_REPLACE );
  //g_TextureManager.getByName( "rttTmp" )->bind();
  //g_TextureManager.getByName( "rttRays" )->bind();
  g_TextureManager.getByName( "rttRes" )->bind();

  glBegin( GL_QUADS );
  glTexCoord2f(0., 1.);
  glVertex2f(0, height-1);

  glTexCoord2f(0., 0.);
  glVertex2f(0, 0);

  glTexCoord2f(1., 0.);
  glVertex2f(width-1, 0);

  glTexCoord2f(1., 1.);
  glVertex2f(width-1, height-1);
  glEnd();

  glPopMatrix();
  glMatrixMode( GL_PROJECTION );
  glPopMatrix();
  glMatrixMode( GL_MODELVIEW );

  Renderer::printGLError();

  printInfos();
}
/****************************************************************************/

void
TestApp::printInfos()
{
  font->setColor(1, 1, 1);
  font->printf(0, height-16, "Fps: %.2f", getFPS());

  font->setColor(1, 0, 0);
  font->printf(0, 16, "OpenGL error: %s", Renderer::getGLErrorString());
}
/****************************************************************************/

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
/****************************************************************************/

void
TestApp::mouseButtonDown(const SDL_MouseButtonEvent &m)
{
  if(m.button==SDL_BUTTON_LEFT)
    trackball->startRotation( m.x,  m.y );
}
/****************************************************************************/

void
TestApp::mouseButtonUp(const SDL_MouseButtonEvent &m)
{
  if(m.button==SDL_BUTTON_LEFT)
    trackball->stopRotation();
}
/****************************************************************************/
