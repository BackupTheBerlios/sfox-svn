#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

class VertexBuffer {
private:
  int type;
  unsigned int id;
  unsigned int size;
  void *mem;
  
public:
  //type = GL_ARRAY_BUFFER_ARB or GL_ELEMENT_ARRAY_BUFFER_ARB
  VertexBuffer(int type, int size, int usage, const void *data = NULL);
  ~VertexBuffer();

  /*size is the number of coordinates(1, 2, 3 or 4)*/
  void vertexPointer(int size, int stride, int offset);
  void texCoordPointer(int size, int stride, int offset);
  void normalPointer(int stride, int offset);
  void attribPointer(unsigned int location, int size, unsigned int type,
                     int stride, int offset);

  void drawArrays(int mode, int first, int count);
  void drawElements(int mode, int count);
  void drawElements(int mode); //draw everything

  void bind();

  void lock(int access);
  void unlock();
};

#endif
