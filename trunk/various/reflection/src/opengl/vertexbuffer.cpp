#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>
#include <cassert>

#include "glext.h"
#include "vertexbuffer.h"

#define OFFSET(x) ((char *)NULL + (x))

VertexBuffer::VertexBuffer(int type, int size, int usage, const void *data)
  : type(type), size(size), mem(0)
{
  assert(type==GL_ARRAY_BUFFER_ARB||type==GL_ELEMENT_ARRAY_BUFFER_ARB);
  glGenBuffersARB(1, &id);
  bind();
  glBufferDataARB(type, size, data, usage);
}

VertexBuffer::~VertexBuffer()
{
  glBindBufferARB(type, 0);
  glDeleteBuffersARB(1, &id);
}

void
VertexBuffer::bind()
{
  glBindBufferARB(type, id);
}

void
VertexBuffer::vertexPointer(int size, int stride, int offset)
{
  glVertexPointer(size, GL_FLOAT, stride, OFFSET(offset));
}

void
VertexBuffer::normalPointer(int stride, int offset)
{
  glNormalPointer(GL_FLOAT, stride, OFFSET(offset));
}

void
VertexBuffer::texCoordPointer(int size, int stride, int offset)
{
  glTexCoordPointer(size, GL_FLOAT, stride, OFFSET(offset));
}

void
VertexBuffer::attribPointer(unsigned int location, int size, unsigned int type,
                            int stride, int offset)
{
  glVertexAttribPointerARB(location, size, type, GL_FALSE, stride,
                           OFFSET(offset));
}

void
VertexBuffer::drawArrays(int mode, int first, int count)
{
  glDrawArrays(mode, first, count);
}

void
VertexBuffer::drawElements(int mode, int count)
{
  glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
}

void
VertexBuffer::drawElements(int mode)
{
  glDrawElements(mode, size/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
}

void
VertexBuffer::lock(int access)
{
  assert(!mem&&"Buffer alreaddy locked");
  mem = glMapBufferARB(type, access);
}

void
VertexBuffer::unlock()
{
  assert(mem&&"Buffer not locked");
  glUnmapBufferARB(type);
  mem = 0;
}
