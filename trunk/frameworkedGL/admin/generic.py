import os, re, types, sys, string, shutil, stat, glob
import SCons.Defaults
import SCons.Tool
import SCons.Util
from SCons.Script.SConscript import SConsEnvironment
from SCons.Options import Options, PathOption

def exists(env):
	return true

def generate(env):
	if not env.has_key('CACHEDIR'):
		env['CACHEDIR'] = os.path.join(os.getcwd(),'cache')
        if not os.path.isdir(env['CACHEDIR']):
		os.mkdir(env['CACHEDIR'])
	env.SConsignFile(env['CACHEDIR']+'/scons_signatures')
	env.Alias('configure', None)
