AM_CXXFLAGS=@CFLAGS@ @FT2_CFLAGS@
LIBS=@FT2_LIBS@ @LIBS@ -lrt -lGLU -lGL
#INCLUDES=
lib_LTLIBRARIES = libGLstats.la
libGLstats_la_SOURCES=glstats.cxx LibraryLoader.cxx GLXFuncs.cxx glx.cxx \
                      font.cxx timer.cxx GLObjectList.cxx \
                      GLStatConfig.cxx Global.cxx GLContext.cxx \
		      LoggerManager.cxx UIWindow.cxx
