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
#include "UILoggerDisplay.h"
#include "Global.h"
#include "CounterManager.h"
#include "Counter.h"

FontGL *fontGL;
Timer *timer;
UIWindow *win;
UILoggerDisplay<float> *uiLogger;

void GLStatsInitGL();

void
GLStatsInit()
{
  Timer::init();
  printf("GLStats init...\n");
  fontGL = 0;
  glStatConfig->setFPSSamplingRate(500);

  LoggerSimple<float> *logger = new LoggerSimple<float>("FPS");
  loggerManager.add(logger);

  Counter *fpsCounter = new Counter("FPS");
  counterManager.add(fpsCounter);

  win = new UIWindow;
  win->setSize(160, 90);
  win->setPosition(0, 0);
  win->setCenter(true);

  uiLogger = new UILoggerDisplay<float>;
  uiLogger->setLogger(logger);
  uiLogger->setSize(160, 90);
  uiLogger->setPosition(0, 0);
  win->addChild(uiLogger);
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
  float sampRate = glStatConfig->getFPSSamplingRate();
  size_t curFrame = state->getCurFrame();
  uint64_t curTime = Timer::getTicks();
  state->incCurFrame();
  Counter *counter = counterManager.getCounter("FPS");
  uint64_t lastReset = counter->getLastResetTime();
  counter->incValue();

  LoggerSimple<float> *logger = (LoggerSimple<float> *)loggerManager.getLogger("FPS");
  if(lastReset >= Timer::msecToUsec(sampRate)) {
    float elapsed = Timer::usecToSec(lastReset);
    float fps = counter->getValue()/elapsed;
    logger->addValue(Timer::getTicks(), fps);
    counter->reset();
  }
  fontGL->setColor(1, 1, 0);
  fontGL->printf(0, 16, "%.2f Fps", logger->getLastValue());
  win->draw();

  global.endGLStatsDrawing();
}
