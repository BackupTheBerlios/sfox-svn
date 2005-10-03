def exists(env):
	return true

def generate(env):
	""" Detect the SDL library """

        from SCons.Options import Options
	cachefile = env['CACHEDIR']+'/sdl.cache.py'
        opts = Options(cachefile)
        opts.AddOptions(
                ( 'SDL_CPPPATH', 'flags for the SDL_LIBRARY' ),
                ( 'SDL_LIBPATH', 'flags for the SDL_LIBRARY' ),
                ( 'SDL_FLAGS', 'flags for the SDL_LIBRARY' ),
                ( 'SDL_LIBS', 'flags for the SDL_LIBRARY' )
        )
        opts.Update(env)

	if 'configure' in env['TARGS'] or not env.has_key('SDL_FLAGS'):
		from SCons.Environment import Base
		sdl_env = Base()
		sdl_env.ParseConfig('sdl-config --cflags --libs')
	        sdl_cflags = sdl_env.Dictionary()['CCFLAGS']
        	sdl_include = sdl_env.Dictionary()['CPPPATH']
	        sdl_libpath = sdl_env.Dictionary()['LIBPATH']
        	sdl_lib = sdl_env.Dictionary()['LIBS']

#		conf = SCons.SConf.SConf( env )
#		if not conf.CheckCHeader('Cg/cg.h'):
#			print 'We really need the cg library !'
#			print 'Get ftp://download.nvidia.com/developer/cg/Cg_1.3/Linux/Cg-1.3.0501-0700.i386.tar.gz and unpack it in your root directory'
#			import sys
#			sys.exit(1)
		env['SDL_FLAGS']=sdl_cflags
		env['SDL_LIBS']=sdl_lib
		env['SDL_LIBPATH']=sdl_libpath
		env['SDL_CPPPATH']=sdl_include
		opts.Save(cachefile, env)
	env.AppendUnique(LIBS=env['SDL_LIBS'],
			 CXXFLAGS=env['SDL_FLAGS'],
			 CCFLAGS=env['SDL_FLAGS'],
			 LIBPATH=env['SDL_LIBPATH'],
			 CPPPATH=env['SDL_CPPPATH']
			)
