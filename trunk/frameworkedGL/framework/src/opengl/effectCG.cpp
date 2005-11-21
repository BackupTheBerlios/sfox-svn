#ifdef WIN32
#include <windows.h>
#endif
#include <cassert>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include "effectCG.h"
#include "shaderCG.h"
#include "framework/exception.h"
#include "opengl/texture.h"
#include "opengl/renderer.h"

namespace StarEngine {

  EffectCG::EffectCG()
    : effect(0)
  {
    context = ShaderCG::getContext();
    assert( context );
  }

  EffectCG::~EffectCG()
  {
    if( effect )
      cgDestroyEffect( effect );

  }

  void
  EffectCG::loadSourceFromFile(const char *filename, const char **args)
  {
    effect = cgCreateEffectFromFile(context, filename, args);
    if( !effect ) {
      const char *listing = cgGetLastListing(context);
      std::string mess("EffectCG::loadSourceFromFile(): Unable " \
                       "to create effect \"");
      mess.append(filename);
      mess.append("\" :\n");
      if(listing)
        mess.append(listing);
      throw new Exception(mess);
    }
    Renderer::printCGError();
  }

  CGtechnique
  EffectCG::getAndValidateTechnique(const char *name)
  {
    CGtechnique technique = cgGetNamedTechnique(effect, name);
    if(!technique) {
      std::string mess("EffectCG::getAndValidateTechnique(): Technique \"");
      mess.append(name);
      mess.append("\" not found.");
      throw new Exception(mess);
    }
    if(!cgValidateTechnique(technique)) {
      std::string mess("EffectCG::getAndValidateTechnique(): Technique \"");
      mess.append(name);
      mess.append("\" didn't validate");
      throw new Exception(mess);
    }
    return technique;
  }


  CGparameter
  EffectCG::getNamedParameter(const char *name)
  {
    return cgGetNamedEffectParameter(effect, name);
  }

  void
  EffectCG::setManageTextureParameters(bool enable)
  {
    cgGLSetManageTextureParameters(ShaderCG::getContext(), enable);
  }

  void
  EffectCG::enableTextureParameter(const char *name)
  {
    cgGLEnableTextureParameter(getNamedParameter( name ));
  }

  void
  EffectCG::disableTextureParameter(const char *name)
  {
    cgGLDisableTextureParameter(getNamedParameter( name ));
  }

  void
  EffectCG::setSamplerState(CGparameter p)
  {
    cgSetSamplerState(p);
  }

  CGprogram
  EffectCG::createProgramFromEffect(CGparameter p, char *progName,
                                    const char** args)
  {
    return cgCreateProgramFromEffect(effect, CG_PROFILE_GENERIC, progName, args);
  }

  void
  EffectCG::evaluateProgram(CGprogram prog, float *obuf, int ncomp,
                              int nx, int ny, int nz)
  {
    cgEvaluateProgram(prog, obuf, ncomp, nx, ny, nz);
  }

/*****************************************************************************/
/* Uniforms setters                                                          */
/*****************************************************************************/
  void
  EffectCG::setGLMVPMatrix( const char *name )
  {
    CGparameter mvp = getNamedParameter( name );
    cgGLSetStateMatrixParameter( mvp, CG_GL_MODELVIEW_PROJECTION_MATRIX,
                                 CG_GL_MATRIX_IDENTITY );
  }

  void
  EffectCG::setTextureParameter(const char *name, Texture *tex)
  {
    cgGLSetTextureParameter(getNamedParameter( name ), tex->getGLName());
  }

  void
  EffectCG::setParameter1f(const char *name,
                           float x)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter1f( param,  x );
  }

  void
  EffectCG::setParameter2f(const char *name,
                           float x,
                           float y)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter2f( param,  x, y );
  }

  void
  EffectCG::setParameter3f(const char *name,
                           float x,
                           float y,
                           float z)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter3f( param,  x, y, z );
  }

  void
  EffectCG::setParameter4f(const char *name,
                           float x,
                           float y,
                           float z,
                           float w)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter4f( param,  x, y, z, w );
  }

  void
  EffectCG::setParameter1fv(const char *name, const float *v)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter1fv( param,  v );
  }

  void
  EffectCG::setParameter2fv(const char *name, const float *v)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter2fv( param,  v );
  }

  void
  EffectCG::setParameter3fv(const char *name, const float *v)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter3fv( param,  v );
  }

  void
  EffectCG::setParameter4fv(const char *name, const float *v)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter4fv( param,  v );
  }

  void
  EffectCG::setParameter1d(const char *name,
                           double x)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter1d( param,  x );
  }

  void
  EffectCG::setParameter2d(const char *name,
                           double x,
                           double y)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter2d( param,  x, y );
  }

  void
  EffectCG::setParameter3d(const char *name,
                           double x,
                           double y,
                           double z)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter3d( param,  x, y, z );
  }

  void
  EffectCG::setParameter4d(const char *name,
                           double x,
                           double y,
                           double z,
                           double w)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter4d( param,  x, y, z, w );
  }

  void
  EffectCG::setParameter1dv(const char *name, const double *v)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter1dv( param,  v );
  }

  void
  EffectCG::setParameter2dv(const char *name, const double *v)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter2dv( param,  v );
  }

  void
  EffectCG::setParameter3dv(const char *name, const double *v)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter3dv( param,  v );
  }

  void
  EffectCG::setParameter4dv(const char *name, const double *v)
  {
    CGparameter param = getNamedParameter( name );
    cgGLSetParameter4dv( param,  v );
  }
};
