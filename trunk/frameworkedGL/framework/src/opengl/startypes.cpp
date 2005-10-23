#include <cassert>

#include "startypes.h"

namespace StarEngine {
  int getSizeOfType(Type type)
  {
    switch(type) {
    case SE_UNSIGNED_BYTE:
      return sizeof(char);
    case SE_UNSIGNED_SHORT:
      return sizeof(unsigned short);
    case SE_UNSIGNED_INT:
      return sizeof(unsigned int);
    default:
      assert(0);
      break;
    }
    return -1;
  }
}
