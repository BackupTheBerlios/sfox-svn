# Inspired by:
# Julien Antille, Thomas Nagy, 2005
# BSD license (see COPYING)

def exists(env):
	return true

def generate(env):
	""" Detect the cg library (nvidia) """

        from SCons.Options import Options
	cachefile = env['CACHEDIR']+'/cg.cache.py'
        opts = Options(cachefile)
        opts.AddOptions(
                ( 'HASCG', 'has the cg library' ),
        )
        opts.Update(env)

	if 'configure' in env['TARGS'] or not env.has_key('HASCG'):
		import SCons.SConf
		conf = SCons.SConf.SConf( env )
		if not conf.CheckCHeader('Cg/cg.h'):
			print 'We really need the cg library !'
			print 'Get ftp://download.nvidia.com/developer/cg/Cg_1.3/Linux/Cg-1.3.0501-0700.i386.tar.gz and unpack it in your root directory'
			import sys
			sys.exit(1)
		env['HASCG']=1
		env = conf.Finish()
		opts.Save(cachefile, env)
	env.AppendUnique(LIBS = ['Cg', 'CgGL', 'GL'])
