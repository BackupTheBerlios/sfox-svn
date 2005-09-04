#ifdef WIN32
#include <windows.h>
#endif
#include <cassert>
#include <fstream>
#include <string>
#include <GL/gl.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include "shaderCG.h"
#include "framework/exception.h"
#include "opengl/texture.h"

namespace StarEngine {
  CGcontext ShaderCG::context = 0;

  void
  ShaderCG::init() {
    if ( !context )
      context = cgCreateContext();
  }

  void
  ShaderCG::quit() {
    if ( context )
      cgDestroyContext(context);
  }

  ShaderCG::ShaderCG()
    : program(0)
  {
    assert( context );
  }

  ShaderCG::~ShaderCG()
  {
    if( program )
      cgDestroyProgram( program );

  }

  void
  ShaderCG::loadSourceFromFile(const char *filename, ProfileType profile,
                               const char *entry,  const char **args)
  {
    profileCG = cgGLGetLatestProfile( getProfileCGGL( profile ) );
    if( profileCG == CG_PROFILE_UNKNOWN )
      throw new Exception("ShaderCG::loadSourceFromFile(): Can't find " \
                          "appropriate profile");
    cgGLSetOptimalOptions( profileCG );
    program = cgCreateProgramFromFile( context, CG_SOURCE, filename, profileCG,
                                       entry, args );
    if ( !program ) {
      std::string mess("ShaderCG::loadSourceFromFile(): Can't load " \
                       "file \"");
      mess.append(filename);
      mess.append("\"");
      throw new Exception(mess);
    }
    cgGLLoadProgram( program );
  }

  void
  ShaderCG::bind()
  {
    cgGLEnableProfile(profileCG);
    cgGLBindProgram(program);
  }

  void
  ShaderCG::unbind()
  {
    cgGLDisableProfile(profileCG);
  }

  CGGLenum
  ShaderCG::getProfileCGGL( ProfileType profile )
  {
    switch( profile ) {
    case VERTEX:
      return CG_GL_VERTEX;
    case FRAGMENT:
      return CG_GL_FRAGMENT;
    default:
      assert( 0 );
    }
    assert(0);
  }

  CGparameter
  ShaderCG::getNamedParameter(const char *name)
  {
    return cgGetNamedParameter(program, name);
  }

  void
  ShaderCG::setGLMVPMatrix( const char *name )
  {
    CGparameter mvp = getNamedParameter( name );
    cgGLSetStateMatrixParameter( mvp, CG_GL_MODELVIEW_PROJECTION_MATRIX,
                                 CG_GL_MATRIX_IDENTITY );
  }

  void
  ShaderCG::setTextureParameter(const char *name, Texture *tex)
  {
    cgGLSetTextureParameter(getNamedParameter( name ), tex->getGLName());
  }

  void
  ShaderCG::enableTextureParameter(const char *name)
  {
    cgGLEnableTextureParameter(getNamedParameter( name ));
  }

  void
  ShaderCG::disableTextureParameter(const char *name)
  {
    cgGLDisableTextureParameter(getNamedParameter( name ));
  }

};
