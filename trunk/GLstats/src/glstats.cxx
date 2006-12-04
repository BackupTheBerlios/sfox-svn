#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <iostream>

#include "BasicException.h"

#include "glstats.h"
#include "font.h"
#include "Logger.h"
#include "LoggerManager.h"
#include "GLStatConfig.h"
#include "GLContext.h"
#include "UIWindow.h"
#include "Global.h"

FontGL *fontGL;
Timer *timer;
UIWindow *win;

void GLStatsInitGL();

void
GLStatsInit()
{
  Timer::init();
  printf("GLStats init...\n");
  fontGL = 0;
  glStatConfig->setFPSSamplingRate(50);

  LoggerSimple<float> *logger = new LoggerSimple<float>("FPS");
  loggerManager.add(logger);

  win = new UIWindow;
  win->setSize(160, 90);
  win->setPosition(0, 0);
  win->setCenter(true);
}

void
GLStatsExit()
{
  printf("GLStats Exit...\n");
  delete fontGL;
  FontGL::exit();
}

void
GLStatsInitGL()
{
  try {
    if(!fontGL) {
      fontGL = new FontGL("data/fonts/DejaVuLGCSansMono-Bold.ttf", 32);
      timer = new Timer();
    }
  } catch(BasicException &e) {
    std::cerr << "Error while initialising GL code:\n" << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
}

void
GLStatsSwapBuffers()
{
  global.beginGLStatsDrawing();

  GLContext *ctx = GLContext::getCurrent();
  GLState *state = ctx->getState();
  size_t sampRate = glStatConfig->getFPSSamplingRate();
  size_t curFrame = state->getCurFrame();
  state->incCurFrame();

  LoggerSimple<float> *logger = (LoggerSimple<float> *)loggerManager.getLogger("FPS");
  if((curFrame+1)%sampRate == 0) {
    float elapsed = Timer::toSec(timer->getElapsed());
    float fps = sampRate/elapsed;
    logger->addValue(Timer::getTicks(), fps);
    timer->start();
  }
  fontGL->setColor(1, 1, 0);
  fontGL->printf(0, 16, "%.2f Fps", logger->getLastValue());
  win->draw();

  global.endGLStatsDrawing();
}
