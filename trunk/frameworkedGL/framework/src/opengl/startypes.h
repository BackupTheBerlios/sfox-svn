#ifndef STARTYPES_H
#define STARTYPES_H

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glew.h>

namespace StarEngine {
  enum PrimitiveMode {
    PM_POINTS,
    PM_LINE_STRIP,
    PM_LINE_LOOP,
    PM_LINES,
    PM_TRIANGLE_STRIP,
    PM_TRIANGLE_FAN,
    PM_TRIANGLES,
    PM_QUAD_STRIP,
    PM_QUADS,
    PM_POLYGON,
    PM_UNKNOWN
  };

  enum Type {
    SE_UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    SE_UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    SE_UNSIGNED_INT = GL_UNSIGNED_INT
  };

  enum UsageType {
    UT_STREAM_DRAW = GL_STREAM_DRAW_ARB,
    UT_STREAM_READ = GL_STREAM_READ_ARB,
    UT_STREAM_COPY = GL_STREAM_COPY_ARB,
    UT_STATIC_DRAW = GL_STATIC_DRAW_ARB,
    UT_STATIC_READ = GL_STATIC_READ_ARB,
    UT_STATIC_COPY = GL_STATIC_COPY_ARB,
    UT_DYNAMIC_DRAW = GL_DYNAMIC_DRAW_ARB,
    UT_DYNAMIC_READ = GL_DYNAMIC_READ_ARB,
    UT_DYNAMIC_COPY = GL_DYNAMIC_COPY_ARB
  };

  enum AccessType {
    AT_READ_ONLY = GL_READ_ONLY_ARB,
    AT_WRITE_ONLY = GL_WRITE_ONLY_ARB,
    AT_READ_WRITE = GL_READ_WRITE_ARB
  };

  int getSizeOfType(Type type);
}

#endif
