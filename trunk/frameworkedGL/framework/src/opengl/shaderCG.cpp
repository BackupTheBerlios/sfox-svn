#ifdef _WIN32
#include <windows.h>
#endif
#include <cassert>
#include <fstream>
#include <string>
#include <GL/gl.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include "shaderCG.h"
#include "glext.h"
#include "framework/exception.h"

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
    CGprofile profileCG = cgGLGetLatestProfile( getProfileCGGL( profile ) );
    if( profileCG == CG_PROFILE_UNKNOWN )
      throw new Exception("ShaderCG::loadSourceFromFile(): Can't find " \
                          "appropriate profile");
    cgGLSetOptimalOptions( profileCG );
    program = cgCreateProgramFromFile( context, CG_SOURCE, filename, profileCG,
                                       entry, args );
    if ( !program ) {
      throw new Exception("ShaderCG::loadSourceFromFile(): Can't load " \
                          "file");
    }
    cgGLLoadProgram( program );
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
  }

};
