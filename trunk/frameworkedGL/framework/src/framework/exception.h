#ifndef EXCEPTION_H
#define EXCEPTION_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <string>
#include <iostream>
#include <SDL.h>

namespace StarEngine {
  class Exception {
  protected:
    std::string str;
  public:
    Exception(const char *m) { str.assign(m); }
    Exception(const std::string &m) { str.assign(m); }

    void show() { std::cerr << str << std::endl; }
  };

  class SDLException : public Exception {
  public:
    SDLException(const char *m) : Exception(m) {
      str.append(SDL_GetError());
    }
  };
}
#endif
