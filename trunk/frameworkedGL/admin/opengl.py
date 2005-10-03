
def exists(env):
	return true

def generate(env):
	""" Detect the openGL library """

        from SCons.Options import Options
	cachefile = env['CACHEDIR']+'/opengl.cache.py'
        opts = Options(cachefile)
        opts.AddOptions(
                ( 'HASGL', 'has the openGL library' ),
        )
        opts.Update(env)

	if 'configure' in env['TARGS'] or not env.has_key('HASGL'):
		import SCons.SConf
		conf = SCons.SConf.SConf( env )
		if not conf.CheckCHeader('GL/gl.h'):
			print 'We really need the openGL library !'
			import sys
			sys.exit(1)
		env['HASGL']=1
		env = conf.Finish()
		opts.Save(cachefile, env)
	env.AppendUnique(LIBS=['GL', 'GLU'], 
			 LIBPATH=['/usr/lib', '/usr/X11R6/lib'],
			 CPPPATH=['/usr/include', '/usr/X11R6/include']
                        )
