SUBDIRS = framework opengl
CXXFLAGS=@SDL_CFLAGS@ @GL_CFLAGS@ @FT2_CFLAGS@ @CXXFLAGS@
LIBS=@SDL_LIBS@ @GL_LIBS@ @FT2_LIBS@ @LIBS@
INCLUDES=-I$(top_srcdir)/src/framework -I$(top_srcdir)/src/opengl -I$(top_srcdir)/src/math

EXTRA_PROGRAMS = reflection reflection.exe
bin_PROGRAMS = @TARGETS@

reflection_SOURCES=main.cpp testapp.cpp testapp.h

reflection_LDADD=$(top_builddir)/src/framework/libframework.a $(top_builddir)/src/opengl/libopengl.a

#For win32 we want exe
reflection_exe_SOURCES=starmain.c
reflection_exe_LDADD=$(top_builddir)/src/framework/libframework.a $(top_builddir)/src/opengl/libopengl.a

mostlyclean-local:
	@-rm -f *~
maintainer-clean-local:
	@-rm -f Makefile.in config.h.in stamp-h.in
