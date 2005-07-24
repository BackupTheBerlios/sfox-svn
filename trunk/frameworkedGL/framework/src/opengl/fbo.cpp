#include "fbo.h"
#include "texture.h"
#include "exception.h"

namespace StarEngine {
  FramebufferObject::FramebufferObject()
  {
    glGenFramebuffersEXT(1, &id);
  }

  FramebufferObject::~FramebufferObject()
  {
    glDeleteFramebuffersEXT(1,  &id);
  }


  void
  FramebufferObject::attachTexture(Texture *tex, AttachmentType attach,
                                   int level)
  {
    GLenum attachType = getGLAttachmentType( attach );
    GLenum texType = tex->getGLTarget();
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachType, texType,
                              tex->getGLName(), level);

  }

  GLenum
  FramebufferObject::getGLAttachmentType( AttachmentType attach )
  {
    if ( attach >= COLOR_ATTACHMENT0 && attach <= COLOR_ATTACHMENT15 )
      return GL_COLOR_ATTACHMENT0_EXT+attach;

    switch( attach ) {
    case DEPTH_ATTACHMENT:
      return GL_DEPTH_ATTACHMENT_EXT;
    case STENCIL_ATTACHMENT:
      return GL_STENCIL_ATTACHMENT_EXT;
    default:
      assert(0);
    }
  }

  void
  FramebufferObject::checkStatus()
  {
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status) {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
      throw new Exception("Unsupported framebuffer format");
      break;
    default:
      assert( 0 );
    }
  }
}
