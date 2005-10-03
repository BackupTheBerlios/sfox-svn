import SCons.SConf

def exists(env):
	return true

def generate(env):
	""" Detect the SDL_Image library """

        from SCons.Options import Options
	cachefile = env['CACHEDIR']+'/sdl_image.cache.py'
        opts = Options(cachefile)
        opts.AddOptions(
                ( 'SDL_IMAGE_CPPPATH', 'flags for the SDL_LIBRARY' ),
                ( 'SDL_IMAGE_LIBPATH', 'flags for the SDL_LIBRARY' ),
        )
        opts.Update(env)

	if 'configure' in env['TARGS'] or not env.has_key('SDL_IMAGE_CPPPATH'):
                conf = SCons.SConf.SConf( env )
		if not conf.CheckLib('SDL_image', symbol="IMG_Load"):
                        print 'We really need the SDL_image library !'
			print 'Get SDL_image'
			import sys
			sys.exit(1)
		env['SDL_IMAGE_CPPPATH']=['']
		env['SDL_IMAGE_LIBPATH']=['']
		opts.Save(cachefile, env)
	env.AppendUnique(LIBS=['SDL_image'],
			 LIBPATH=env['SDL_IMAGE_LIBPATH'],
			 CPPPATH=env['SDL_IMAGE_CPPPATH']
			)
