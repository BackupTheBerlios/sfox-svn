#ifndef APPLICATION_H
#define APPLICATION_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <cassert>
#include <SDL.h>

#include "exception.h"
#include "timer.h"

class Application {
protected:
  int width, height;

private:
  bool keys[SDLK_LAST];
  
  bool fullscreen;
  bool finish;

  int exitCode;

  int numFrame;
  static const int NEEDEDFRAMES = 20;
  float fps;
  float renderTime;
  Timer timer;

public:
  Application(int resx, int resy, const char *name, bool fs = false);
  virtual ~Application() {}
  
  int run();
  void halt(int code) { exitCode = code; finish = true; }

  virtual void init() = 0;
  virtual void render() = 0;
  virtual void idle() = 0;
  virtual void quit() {}

  virtual void mouseButtonDown(const SDL_MouseButtonEvent &m) {}
  virtual void mouseButtonUp(const SDL_MouseButtonEvent &m) {}
  virtual void keyDown(const SDL_keysym &key) = 0;
  virtual void keyUp(const SDL_keysym &key) = 0;
  virtual void mouseMotion(const SDL_MouseMotionEvent &motion) {}

  float getFPS() { return fps; }

  bool isDown(SDLKey k) {
    assert(k<SDLK_LAST);
    return keys[k];
  }

  SDLMod getModState() {
    return SDL_GetModState();
  }

  void warpMouse(unsigned int x, unsigned int y) {
    SDL_WarpMouse(x, y);
  }

  void grabInput(bool on) {
    SDL_GrabMode flag = on?SDL_GRAB_ON:SDL_GRAB_OFF;
    SDL_WM_GrabInput(flag);
  }

  void hideCursor(bool on) {
    unsigned int flag = on?SDL_DISABLE:SDL_ENABLE;
    SDL_ShowCursor(flag);
  }

  void setFullscreen(bool fs);
};

#endif
