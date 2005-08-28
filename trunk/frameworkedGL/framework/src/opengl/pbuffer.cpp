#ifdef WIN32
#include <windows.h>
#else
#include <GL/glx.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <SDL/SDL_syswm.h>
#include <string>
#include <vector>
#include <cstdlib>

#include "pbuffer.h"
#include "framework/exception.h"

namespace StarEngine {
  PBuffer::PBuffer(int width, int height, const std::string &format)
  {
#ifndef WIN32
    using namespace std;
    const vector<int> configs = extractFormat(format);
    int n, i;
    int configAttribs [] = {
      //GLX_FLOAT_COMPONENTS_NV, True,
      GLX_RED_SIZE, 8,
      GLX_GREEN_SIZE, 8,
      GLX_BLUE_SIZE, 8,
      GLX_DEPTH_SIZE, 16,
      //GLX_ALPHA_SIZE, 8,
      GLX_DRAWABLE_TYPE_SGIX, GLX_PBUFFER_BIT_SGIX,
      (int) None
    };

    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    SDL_GetWMInfo(&info);
    display = info.info.x11.display;

    GLXFBConfig *config = glXChooseFBConfig(display, 0, &configs[0], &n);
    for(i = 0; i < n; i++) {
      //pbuffer = glXCreatePbuffer(info.info.x11.display, config[i], width, height, NULL);
      pbuffer = glXCreatePbuffer(info.info.x11.display, config[i], NULL);
      if(pbuffer)
        break;
    }
    if(!pbuffer)
      throw new Exception("Can't create pbuffer!");

    oldContext = glXGetCurrentContext();
    oldDrawable = glXGetCurrentDrawable();

    context =  glXCreateNewContext(display, config[i], GLX_RGBA_TYPE,
                                   oldContext, True);
    if(!context)
      throw new Exception("Can't create new context for pbuffer!");

    this->width = width;
    this->height = height;
    //glXQueryPbuffer(display, pbuffer, GLX_WIDTH_SGIX, &this->width);
    //glXQueryPbuffer(display, pbuffer, GLX_HEIGHT_SGIX, &this->height);
    std::cout << width << "x" << height << std::endl;
#endif
  }

  void
  PBuffer::enable()
  {
#ifndef WIN32
    oldContext = glXGetCurrentContext();
    oldDrawable = glXGetCurrentDrawable();
    if(glXMakeCurrent(display, pbuffer, context) == False)
      throw new Exception("Can't activate pbuffer!");
#endif
  }

  void
  PBuffer::disable()
  {
#ifndef WIN32
    if(glXMakeCurrent(display, oldDrawable, oldContext) == False)
      throw new Exception("Can't disable pbuffer!");
#endif
  }

  PBuffer::~PBuffer()
  {
#ifndef WIN32
    if(glXGetCurrentContext() == context)
      glXMakeCurrent(display, oldDrawable, oldContext);
    glXDestroyPbuffer(display, pbuffer);
#endif
  }

  const std::vector<int>
  PBuffer::extractFormat(const std::string &format)
  {
    using namespace std;
    string::const_iterator i;
    vector<string> conf;
    vector<int> fbAttribs;
    string tmp;

#ifndef WIN32
    // Extract each word from the string "xxx yy ..."
    for(i = format.begin(); i != format.end(); i++) {
      char c = *i;
      if(c != ' ')
        tmp.append(1, c);
      else if(!tmp.empty()) {
        conf.push_back(tmp);
        tmp.clear();
      }
    }
    if(!tmp.empty())
      conf.push_back(tmp);

    // Convert string parameters to GLX attribs
    for(vector<string>::iterator i = conf.begin(); i!= conf.end(); i++) {
      const string &flag = *i;
      if(flag.compare("rgba") == 0) {
        fbAttribs.push_back(GLX_RED_SIZE);
        fbAttribs.push_back(8);
        fbAttribs.push_back(GLX_GREEN_SIZE);
        fbAttribs.push_back(8);
        fbAttribs.push_back(GLX_BLUE_SIZE);
        fbAttribs.push_back(8);
        fbAttribs.push_back(GLX_ALPHA_SIZE);
        fbAttribs.push_back(8);
      } else if(flag.compare("r") == 0) {
        fbAttribs.push_back(GLX_RED_SIZE);
        fbAttribs.push_back(8);
      } else if(flag.compare("g") == 0) {
        fbAttribs.push_back(GLX_GREEN_SIZE);
        fbAttribs.push_back(8);
      } else if(flag.compare("b") == 0) {
        fbAttribs.push_back(GLX_BLUE_SIZE);
        fbAttribs.push_back(8);
      } else if(flag.compare("a") == 0) {
        fbAttribs.push_back(GLX_ALPHA_SIZE);
        fbAttribs.push_back(8);
      } else if(flag.compare("rgb") == 0) {
        fbAttribs.push_back(GLX_RED_SIZE);
        fbAttribs.push_back(8);
        fbAttribs.push_back(GLX_GREEN_SIZE);
        fbAttribs.push_back(8);
        fbAttribs.push_back(GLX_BLUE_SIZE);
        fbAttribs.push_back(8);
      } else if(flag.compare(0, strlen("depth="), "depth=") == 0) {
        int bits = atoi(flag.substr(strlen("depth=")).c_str());
        fbAttribs.push_back(GLX_DEPTH_SIZE);
        fbAttribs.push_back(bits);
        printf("%d\n",bits);
      } else if(flag.compare("depth") == 0) {
        fbAttribs.push_back(GLX_DEPTH_SIZE);
        fbAttribs.push_back(16);
      }
    }
    fbAttribs.push_back(GLX_DRAWABLE_TYPE_SGIX);
    fbAttribs.push_back(GLX_PBUFFER_BIT_SGIX);
    fbAttribs.push_back((int)None);
#endif

    return fbAttribs;
  }
}
