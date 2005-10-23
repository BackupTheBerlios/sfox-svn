#ifdef WIN32
#include <windows.h>
#endif
#include <cassert>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include "shaderCG.h"
#include "framework/exception.h"
#include "opengl/texture.h"
#include "opengl/renderer.h"

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
                               const CGenum program_type,
                               const char *entry,  const char **args)
  {
    profileCG = cgGLGetLatestProfile( getProfileCGGL( profile ) );
    if( profileCG == CG_PROFILE_UNKNOWN )
      throw new Exception("ShaderCG::loadSourceFromFile(): Can't find " \
                          "appropriate profile");
    cgGLSetOptimalOptions( profileCG );
    program = cgCreateProgramFromFile( context, CG_SOURCE, filename, profileCG,
                                       entry, args );
    Renderer::printCGError();

    if ( !program ) {
      std::string mess("ShaderCG::loadSourceFromFile(): Can't load " \
                       "file \"");
      mess.append(filename);
      mess.append("\"");
      throw new Exception(mess);
    }
    cgGLEnableProfile(profileCG);
    cgGLLoadProgram( program );
    Renderer::printCGError();
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
  ShaderCG::enableTextureParameter(const char *name)
  {
    cgGLEnableTextureParameter(getNamedParameter( name ));
  }

  void
  ShaderCG::disableTextureParameter(const char *name)
  {
    cgGLDisableTextureParameter(getNamedParameter( name ));
  }

/*****************************************************************************/
/* Uniforms setters                                                          */
/*****************************************************************************/
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
  ShaderCG::setParameter1f(const char *name,
                           float x)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter1f( param,  x );
  }

  void
  ShaderCG::setParameter2f(const char *name,
                           float x,
                           float y)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter2f( param,  x, y );
  }

  void
  ShaderCG::setParameter3f(const char *name,
                           float x,
                           float y,
                           float z)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter3f( param,  x, y, z );
  }

  void
  ShaderCG::setParameter4f(const char *name,
                           float x,
                           float y,
                           float z,
                           float w)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter4f( param,  x, y, z, w );
  }

  void
  ShaderCG::setParameter1fv(const char *name, const float *v)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter1fv( param,  v );
  }

  void
  ShaderCG::setParameter2fv(const char *name, const float *v)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter2fv( param,  v );
  }

  void
  ShaderCG::setParameter3fv(const char *name, const float *v)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter3fv( param,  v );
  }

  void
  ShaderCG::setParameter4fv(const char *name, const float *v)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter4fv( param,  v );
  }

  void
  ShaderCG::setParameter1d(const char *name,
                           double x)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter1d( param,  x );
  }

  void
  ShaderCG::setParameter2d(const char *name,
                           double x,
                           double y)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter2d( param,  x, y );
  }

  void
  ShaderCG::setParameter3d(const char *name,
                           double x,
                           double y,
                           double z)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter3d( param,  x, y, z );
  }

  void
  ShaderCG::setParameter4d(const char *name,
                           double x,
                           double y,
                           double z,
                           double w)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter4d( param,  x, y, z, w );
  }

  void
  ShaderCG::setParameter1dv(const char *name, const double *v)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter1dv( param,  v );
  }

  void
  ShaderCG::setParameter2dv(const char *name, const double *v)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter2dv( param,  v );
  }

  void
  ShaderCG::setParameter3dv(const char *name, const double *v)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter3dv( param,  v );
  }

  void
  ShaderCG::setParameter4dv(const char *name, const double *v)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter4dv( param,  v );
  }
};
