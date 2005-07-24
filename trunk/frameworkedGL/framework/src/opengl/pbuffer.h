#ifndef PBUFFER_H
#define PBUFFER_H

#include <string>
#include <vector>

namespace StarEngine {
  class PBuffer {
  private:
#ifndef _WIN32
    Display *display;
    GLXPbuffer pbuffer;
    GLXContext context;
    GLXContext oldContext;
    GLXDrawable oldDrawable;
#endif

    const std::vector<int> extractFormat(const std::string &format);

  public:
    PBuffer(int width, int height, const std::string &format);
    ~PBuffer();

    void enable();
    void disable();

    unsigned int width, height;

  };
}
#endif
