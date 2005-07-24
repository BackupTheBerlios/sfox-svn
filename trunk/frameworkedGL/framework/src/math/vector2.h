#ifndef VECTOR2_H
#define VECTOR2_H

namespace StarEngine {
  template<class T>
  class Vector2 {
  public:
    T x,y;
    Vector2() {}
    Vector2(T x, T y):x(x), y(y) {}
    ~Vector2() {}
  };

  typedef Vector2<float> Vec2f;
}

#endif
