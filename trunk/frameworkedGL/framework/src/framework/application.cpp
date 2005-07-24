#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>
#include <GL/gl.h>
#include <SDL.h>
#include <cstring>

#include "application.h"
#include "texturemanager.h"

namespace StarEngine {
  using namespace std;

  Application::Application(int resx, int resy, const char *name, bool fs)
    : width(resx), height(resy), finish(false), numFrame(0), fps(0.0f)
  {
    memset(keys, 0, sizeof(keys));
    if( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_NOPARACHUTE) == -1 )
      throw new SDLException("Erreur d'initialisation de la SDL: ");

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    setFullscreen(fs);
    SDL_WM_SetCaption(name, NULL);

    // Create ressources managers
    new TextureManager;
  }

  void
  Application::setFullscreen(bool fs)
  {
    fullscreen = fs;
    int flags = SDL_OPENGL;
    if( fullscreen )
      flags |= SDL_FULLSCREEN;

    if(!SDL_SetVideoMode(width, height, 32, flags)) {
      SDL_Quit();
      throw new SDLException("Can't set video mode: ");
    }
  }

  int
  Application::run()
  {
    SDL_Event event;
    init();
    while(!finish) {
      while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_KEYDOWN:
          keys[event.key.keysym.sym] = true;
          keyDown(event.key.keysym);
          break;
        case SDL_KEYUP:
          keys[event.key.keysym.sym] = false;
          keyUp(event.key.keysym);
          break;
        case SDL_MOUSEMOTION:
          mouseMotion(event.motion);
          break;
        case SDL_MOUSEBUTTONDOWN:
          mouseButtonDown(event.button);
          break;
        case SDL_MOUSEBUTTONUP:
          mouseButtonUp(event.button);
          break;
        default:
          break;
        }
      }
      if(numFrame++ >= NEEDEDFRAMES) {
        fps = (NEEDEDFRAMES*1000.0f)/(float)timer.getElapsed();
        numFrame = 0;
        timer.start();
      }
      idle();
      render();
      SDL_GL_SwapBuffers();
    }
    quit();
    SDL_WM_GrabInput(SDL_GRAB_OFF);
    SDL_ShowCursor(SDL_ENABLE);
    SDL_Quit();
    return exitCode;
  }
}
