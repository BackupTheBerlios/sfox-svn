#ifndef FBO_H
#define FBO_H

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>

namespace StarEngine {
  class Texture;
}

namespace StarEngine {
  class FramebufferObject {
  private:
    GLuint id;

  public:
    enum AttachmentType {
      COLOR_ATTACHMENT0 = 0,
      COLOR_ATTACHMENT1,
      COLOR_ATTACHMENT2,
      COLOR_ATTACHMENT3,
      COLOR_ATTACHMENT4,
      COLOR_ATTACHMENT5,
      COLOR_ATTACHMENT6,
      COLOR_ATTACHMENT7,
      COLOR_ATTACHMENT8,
      COLOR_ATTACHMENT9,
      COLOR_ATTACHMENT10,
      COLOR_ATTACHMENT11,
      COLOR_ATTACHMENT12,
      COLOR_ATTACHMENT13,
      COLOR_ATTACHMENT14,
      COLOR_ATTACHMENT15,
      DEPTH_ATTACHMENT,
      STENCIL_ATTACHMENT
    };

    FramebufferObject();
    ~FramebufferObject();

    void bind() const;
    static void unbind();

    void attachTexture(Texture *tex, AttachmentType attach, int level);
    void detachTexture(Texture *tex, AttachmentType attach, int level);
    void checkStatus();

  private:
    GLenum getGLAttachmentType( AttachmentType attach );
  };
}

#endif
