#ifndef UTILS_H
#define UTILS_H

#ifdef _WIN32
#define M_PI       3.14159265358979323846f
#endif

#include <cmath>

namespace Utils {
  template<typename T>
  T Deg2Rad(T x) { return (x*M_PI)/T(180); }

  // Clamp x into [a,b]
  template<typename T>
  T clamp(T x, T a, T b) {
    return std::min(std::max(a, x), b);
  }

  template<typename T>
  T Deg360Bound(T x) {
    return ((x>=360)?x-360:((x<=-360)?x+360:x));
  }
  
};

#endif
