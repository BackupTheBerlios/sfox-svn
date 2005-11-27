import os, re, types, sys, string, shutil, stat, glob
import SCons.Defaults
import SCons.Tool
import SCons.Util
from SCons.Script.SConscript import SConsEnvironment
from SCons.Options import Options, PathOption

colors= {
'BOLD'  :"\033[1m",
'RED'   :"\033[91m",
'GREEN' :"\033[92m",
'YELLOW':"\033[1m", #"\033[93m" # unreadable on white backgrounds
'CYAN'  :"\033[96m",
'NORMAL':"\033[0m",
}

def pprint(env, col, str, label=''):
        if env.has_key('NOCOLORS'):
                print "%s %s" % (str, label)
                return
        try: mycol=colors[col]
        except: mycol=''
        print "%s%s%s %s" % (mycol, str, colors['NORMAL'], label)

def exists(env):
	return true

def generate(env):
	SConsEnvironment.pprint = pprint
	if '--help' in sys.argv:
                p=env.pprint
                p('BOLD','*** Instructions ***')
                p('BOLD','--------------------')
                p('BOLD','* scons           ','to compile')
                p('BOLD','* scons -j4       ','to compile with several instances')
		p('BOLD','* scons install   ','to compile and install')
                p('BOLD','* scons -c install','to uninstall')
                p('BOLD','\n*** Generic options ***')
                p('BOLD','--------------------')
                p('BOLD','* debug        ','debug=1 (-g)')
	        p('BOLD','* prefix       ','the installation path')
                p('BOLD','* scons configure prefix=/usr/local')
                p('BOLD','* scons install prefix=/opt/local\n')
                sys.exit(0)


	if not env.has_key('CACHEDIR'):
		env['CACHEDIR'] = os.path.join(os.getcwd(),'cache')
        if not os.path.isdir(env['CACHEDIR']):
		os.mkdir(env['CACHEDIR'])
	env.SConsignFile(env['CACHEDIR']+'/scons_signatures')
	env.Alias('configure', None)

        def makeHashTable(args):
                table = { }
                for arg in args:
                        if len(arg) > 1:
                                lst=arg.split('=')
                                if len(lst) < 2:
					continue
                                key=lst[0]
                                value=lst[1]
                                if len(key) > 0 and len(value) >0:
					table[key] = value
                return table
        env['ARGS']=makeHashTable(sys.argv)

        ## load the options
        cachefile=os.path.join(env['CACHEDIR'],'generic.cache.py')
        opts = Options(cachefile)
	opts.AddOptions(
		( 'PREFIX', 'prefix for installation'),
		( 'DEBUG', 'Enable debug'),
		( 'CONFIGURED', 'True if already configured')
	)
        opts.Update(env)
	
	#Set config options if needed
	if not env.has_key('CONFIGURED') or 'configure' in sys.argv:
		#Get installation prefix
		if env['ARGS'].has_key('prefix'):
			env['PREFIX']=os.path.abspath(env['ARGS'].get('prefix', ''))
		else:
			env['PREFIX']='/usr/local/'
		if env['ARGS'].has_key('debug'):
			env['DEBUG']=env['ARGS'].get('debug', 0)
		else:
			env['DEBUG']='0'
		env['CONFIGURED']=1
		opts.Save(cachefile, env)

	#Set debug options
	if env['DEBUG'] == '1':
		env.AppendUnique(CXXFLAGS='-g -Wall -DDEBUG',
				 CFLAGS='-g -Wall -DDEBUG')
	else:
		env.AppendUnique(CXXFLAGS='--fast-math -O2 -DNDEBUG',
				 CFLAGS='--fast-math -O2 -DNDEBUG')
