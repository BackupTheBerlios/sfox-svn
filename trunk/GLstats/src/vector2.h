#ifndef VECTOR2_H
#define VECTOR2_H

#include <cassert>
#include <cmath>

template<class T>
class Vector2 {
public:
  Vector2() {}

  Vector2(T x, T y):x(x), y(y) {}

  ~Vector2() {}

  T& operator[](unsigned int i) {
    assert(i<2);
    return *(&x+i);
  }

  const T &operator[](unsigned int i) const {
    assert(i<2);
    return *(&x+i);
  }

  void getValue(T &x, T &y) const {
    x = this->x; y = this->y;
  }

  void setValues(T x, T y) {
    this->x = x; this->y = y;
  }

  void setValues(const Vector2<T>& v) {
    x = v.x; y = v.y;
  }

  void setValues(const T v[2]) {
    x = v[0]; y = v[1];
  }

  T maxValue() {
    return (y>x)?y:x;
  }

  Vector2<T> operator-() const {
    return Vector2<T>(-x, -y);
  }

  Vector2<T>& operator=(const Vector2<T> &v) {
    x = v.x; y = v.y;
    return *this;
  }

  Vector2<T>& operator+=(const Vector2<T> &v) {
    x += v.x; y += v.y;
    return *this;
  }

  Vector2<T>& operator-=(const Vector2<T>& v)  {
    x -= v.x; y -= v.y;
    return *this;
  }

  Vector2<T>& operator*=(T k) {
    x *= k; y *= k;
    return *this;
  }

  Vector2<T>& operator/=(T k)  {
    assert(k != 0);
    x /= k; y /= k;
    return *this;
  }

  Vector2<T> operator+(const Vector2<T> &v) const {
    return Vector2<T>(x+v.x, y+v.y);
  }

  Vector2<T> operator-(const Vector2<T> &v) const {
    return Vector2<T>(x-v.x, y-v.y);
  }

  T operator*(const Vector2<T> &v) const {
    return dot(v);
  }

  Vector2<T> operator*(T k) const {
    return Vector2<T>(x*k, y*k);
  }

  Vector2<T> operator/(T k) const {
    assert(k!=0);
    return Vector2<T>(x/k, y/k);
  }

  bool operator==(const Vector2<T> &v) const {
//     using namespace std;
//     return abs(x-v.x)<epsilon && abs(y-v.y)<epsilon;
     using namespace std;
     return x == v.x && y == v.y;
  }

  bool isNull() const {
    return *this == Vector2<T>(0,0,0);
  }

  T dist(const Vector2<T> &v) const {
    return (*this - v).length();
  }

  T dist2(const Vector2<T> &v) const {
    return (*this - v).length2();
  }

  T length() const {
    return std::sqrt(x*x+y*y);
  }

  T length2() const {
    return x*x+y*y;
  }

  void normalize() {
    T l = length();
    if(l == 0)
      x=y=0;
    else
      *this *= 1.0f/l;
  }

  T dot(const Vector2<T> &v) const {
    return x*v.x+y*v.y;
  }

  friend Vector2<T> operator*(T k, const Vector2<T>&v) {
    return v*k;
  }

  friend std::ostream& operator<<(std::ostream& os, const Vector2<T>& v) {
      return os << v.x << ", " << v.y;
  }

private:
  T x, y;
};

typedef Vector2<float> Vec2f;
typedef Vector2<int> Vec2i;

#endif
