CXXFLAGS=@SDL_CFLAGS@ @GL_CFLAGS@ @CXXFLAGS@
noinst_LIBRARIES=libframework.a
libframework_a_SOURCES=application.cpp application.h exception.h\
		       timer.cpp timer.h

mostlyclean-local:
	@-rm -f *~
maintainer-clean-local:
	@-rm -f Makefile.in config.h.in stamp-h.in
