AC_INIT(GLstats, 0.1)

dnl Detect the canonical host and target build environment
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AC_CONFIG_SRCDIR(src/glstats.cxx)
AM_INIT_AUTOMAKE

AC_PROG_CXX
AC_PROG_LIBTOOL

REQUIRED_FREETYPE2_VERSION=2.0.0

AC_C_CONST

dnl Checks for freetype2
AC_CHECK_FT2([$REQUIRED_FREETYPE2_VERSION],, [AC_MSG_ERROR([*** FREETYPE2 version too old])])

dnl Check for libpng
AC_CHECK_LIB(m, pow)
AC_CHECK_LIB(z, gzopen)
AC_CHECK_LIB(png, png_init_io,, [AC_MSG_ERROR(*** libpng not found)], [-lz -lm])

dnl Enable debug or profiling
AC_ARG_ENABLE(debugging, [  --enable-debug          Compile with debug symbols ], CFLAGS="$CFLAGS -g -Wall")
AC_ARG_ENABLE(profiling, [  --enable-profiling      Compile with profiling enabled ], CFLAGS="$CFLAGS -g -pg")
CXXFLAGS="$CXXFLAGS $CFLAGS"

AC_OUTPUT([Makefile \
           src/Makefile])
