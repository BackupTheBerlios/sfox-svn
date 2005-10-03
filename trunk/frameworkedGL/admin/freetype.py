import SCons.SConf

def exists(env):
	return true

def generate(env):
	""" Detect the freetype library """

        from SCons.Options import Options
	cachefile = env['CACHEDIR']+'/freetype.cache.py'
        opts = Options(cachefile)
        opts.AddOptions(
                ( 'FREETYPE_CPPPATH', 'flags for the SDL_LIBRARY' ),
                ( 'FREETYPE_LIBPATH', 'flags for the SDL_LIBRARY' ),
                ( 'FREETYPE_FLAGS', 'flags for the SDL_LIBRARY' ),
                ( 'FREETYPE_LIBS', 'flags for the SDL_LIBRARY' )
        )
        opts.Update(env)

	if 'configure' in env['TARGS'] or not env.has_key('FREETYPE_FLAGS'):
		from SCons.Environment import Base
		freetype_env = Base()
	        freetype_env.ParseConfig('pkg-config --cflags --libs freetype2 || freetype-config --cflags --libs')
        	freetype_cflags = freetype_env.Dictionary()['CCFLAGS']
	        freetype_include = freetype_env.Dictionary()['CPPPATH']
        	freetype_libpath = freetype_env.Dictionary()['LIBPATH']
	        freetype_lib = freetype_env.Dictionary()['LIBS']

		env['FREETYPE_FLAGS']=freetype_cflags
		env['FREETYPE_LIBS']=freetype_lib
		env['FREETYPE_LIBPATH']=freetype_libpath
		env['FREETYPE_CPPPATH']=freetype_include
		opts.Save(cachefile, env)
	env.AppendUnique(LIBS=env['FREETYPE_LIBS'],
			 CXXFLAGS=env['FREETYPE_FLAGS'],
			 CCFLAGS=env['FREETYPE_FLAGS'],
			 LIBPATH=env['FREETYPE_LIBPATH'],
			 CPPPATH=env['FREETYPE_CPPPATH']
			)
