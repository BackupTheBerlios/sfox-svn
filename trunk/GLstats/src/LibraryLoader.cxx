#include "LibraryLoader.h"

#include <dlfcn.h>
#include <sstream>
#include <cassert>

#include "BasicException.h"

void
LibraryLoader::open(const char *libFilename)
{
  using namespace std;

  handle = dlopen(libFilename, RTLD_LAZY);
  if(!handle) {
    ostringstream err;
    err << "Libraryloader::open(): dlopen() error: " << dlerror() << ".";
    throw BasicException(err.str());
  }
}

void *
LibraryLoader::getSym(const char *symName)
{
  assert(handle);
  void *addr = dlsym(RTLD_NEXT, symName);
  char *error = dlerror();
  if(error) {
    std::ostringstream err;
    err << "Libraryloader::getSym(): dlsym() error: " << error << ".";
    throw BasicException(err.str());
  }
  return addr;
}

void
LibraryLoader::close()
{
  assert(handle);
  dlclose(handle);
  handle = 0;
}
