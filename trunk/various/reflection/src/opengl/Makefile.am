CXXFLAGS=@SDL_CFLAGS@ @GL_CFLAGS@ @CXXFLAGS@ @FT2_CFLAGS@
INCLUDES=-I$(top_srcdir)/src/math -I$(top_srcdir)/src/framework
noinst_LIBRARIES=libopengl.a
libopengl_a_SOURCES=light.cpp light.h\
		    texture.cpp texture.h\
		    font.h font.cpp \
		    camera.cpp camera.h \
		    renderer.h renderer.cpp\
		    mesh.h mesh.cpp \
		    vertexbuffer.h vertexbuffer.cpp\
		    glext.h glext.cpp \
		    object3d.h object3d.cpp \
		    material.h material.cpp \
		    program.cpp program.h \
		    shader.cpp shader.h

mostlyclean-local:
	@-rm -f *~
maintainer-clean-local:
	@-rm -f Makefile.in config.h.in stamp-h.in
