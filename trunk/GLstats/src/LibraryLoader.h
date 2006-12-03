#ifndef LIBRARYLOADER_H
#define LIBRARYLOADER_H

class LibraryLoader {
public:
  LibraryLoader():handle(0) {}

  //Throw Basicexception
  void open(const char *libFilename);
  void *getSym(const char *symName);
  void close();

private:
  void *handle;
};

#endif
