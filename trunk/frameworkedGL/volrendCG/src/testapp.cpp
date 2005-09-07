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
#include "opengl/shaderCG.h"
#include "opengl/trackball.h"
#include "opengl/fbo.h"

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
/****************************************************************************/

void
TestApp::quit()
{
  delete font;
  FontGL::exit();
}
/****************************************************************************/
#if 0
unsigned char*
TestApp::genPreIntegrated(Image *img)
{ 
  ImageLoader::ImageData imgData = img->getImageData();
  unsigned char *colMap = (unsigned char *)imgData->data;
  int K[256*4];
  unsigned char *res = new unsigned char[256*256*4];  
  K[0] = colMap[0];
  K[1] = colMap[1];
  K[2] = colMap[2];
  K[3] = colMap[3];    
  for(int i = 1; i < 256; i++) {
    K[i*4] = K[(i-1)*4]+colMap[i*4];
    K[i*4+1] = K[(i-1)*4]+colMap[i*4+1];
    K[i*4+2] = K[(i-1)*4]+colMap[i*4+2];
    K[i*4+3] = K[(i-1)*4]+colMap[i*4+3];
  }

  float rr, rg, rb, ra;
  for(int sf = 0; sf < 256; sf++)
    for(int sb = 0; sb < 256; sb++) {
      if(sf != sb) {
        float k = 1./(sb-sf);
        rr = k*(K[sf*4]-K[sb*4]);
        rg = k*(K[sf*4+1]-K[sb*4+1]);
        rb = k*(K[sf*4+2]-K[sb*4+2]);        
        ra = 1.-exp(-k*(K[sf*4+3]-K[sb*4+3]));
      } else {
        rr = colMap[sf*4];
        rg = colMap[sf*4+1];
        rb = colMap[sf*4+2];
        ra = 1.-exp(-colMap[sf*4+3]);
      }
      rr = min(rr, 255);
      rb = min(rg, 255);
      rg = min(rb, 255);
      ra= min(ra, 255);
      res[(sf*256+sb)*4] = rr;
      res[(sf*256+sb)*4+1] = rg;
      res[(sf*256+sb)*4+2] = rb;
      res[(sf*256+sb)*4+3] = ra;
    }
  return res;
}
#endif
/****************************************************************************/

void
TestApp::init() {
  GLext::init();
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0, 0, 0, 0);

  font = new FontGL(DATAPATH"/media/fonts/vera.ttf", 16, 128);

  cam = new Camera(60.f, 0.001f, 50.f);
  cam->setPos(0, 0, 4);

  trackball = new Trackball(width, height);

  Texture *texture = g_TextureManager.load("volData",
                                           DATAPATH"/media/volume/head256.dat");

  fbo = new FramebufferObject;
  fbo->bind();
  g_TextureManager.load("colorMap", DATAPATH"/media/volume/colormap/bonsai.png");
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

  shaderVertPass1 = new ShaderCG();
  shaderVertPass1->loadSourceFromFile("shaders/pass1.vert", ShaderCG::VERTEX);

  shaderFragPass1 = new ShaderCG();
  shaderFragPass1->loadSourceFromFile("shaders/pass1.frag",
                                      ShaderCG::FRAGMENT);

  shaderFragPass2 = new ShaderCG();
  shaderFragPass2->loadSourceFromFile("shaders/pass2.frag",
                                      ShaderCG::FRAGMENT);
  shaderFragPass3 = new ShaderCG();
  shaderFragPass3->loadSourceFromFile("shaders/pass3.frag",
                                      ShaderCG::FRAGMENT);
  Renderer::printCGError();
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

   shaderVertPass1->bind();
   shaderVertPass1->setGLMVPMatrix("mvp");
   shaderFragPass1->bind();
   drawCube(float(cubeX), float(cubeY), float(cubeZ));
   shaderFragPass1->unbind();
   glFlush();

   // Render back faces and compute rays
   fbo->attachTexture( texRays, FramebufferObject::COLOR_ATTACHMENT0, 0 );
   fbo->checkStatus();
   glClear(GL_COLOR_BUFFER_BIT);
   texTmp->bind();

   glCullFace(GL_FRONT);
   shaderFragPass2->bind();

   shaderFragPass2->setTextureParameter("frontFace", texTmp);
   shaderFragPass2->setParameter2f("winScale", 1./width, 1./height);
   shaderFragPass2->enableTextureParameter("frontFace");
   drawCube(cubeX, cubeY, cubeZ);
   shaderFragPass2->disableTextureParameter("frontFace");
   shaderVertPass1->unbind();
   shaderFragPass2->unbind();

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
   Texture * texColorMap = g_TextureManager.getByName("colorMap");

   texColorMap->bind();
   texColorMap->setMinFilter( TF_NEAREST );
   texColorMap->setMagFilter( TF_NEAREST );

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

   shaderVertPass1->bind();
   shaderFragPass3->bind();
   shaderFragPass3->setParameter1f("dt", 0.01f);
   float t = 0.0f;

   glCullFace( GL_BACK );
   shaderFragPass3->setParameter1f("t", t);
   shaderFragPass3->setParameter2f("winScale", 1./width, 1./height);
   shaderFragPass3->setTextureParameter("raysDir", texRays);
   shaderFragPass3->setTextureParameter("volData", texVolData);
   shaderFragPass3->setTextureParameter("colorMap", texColorMap);
   shaderFragPass3->enableTextureParameter("volData");
   shaderFragPass3->enableTextureParameter("raysDir");
   shaderFragPass3->enableTextureParameter("colorMap");
   drawCube(cubeX, cubeY, cubeZ);
   shaderFragPass3->disableTextureParameter("volData");
   shaderFragPass3->disableTextureParameter("raysDir");
   shaderFragPass3->disableTextureParameter("colorMap");
   shaderFragPass3->unbind();
   shaderVertPass1->unbind();

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
  FramebufferObject::unbind();

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

  Renderer::printCGError();
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
  font->printf(0, 32, "CG error: %s", Renderer::getCGErrorString());
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
