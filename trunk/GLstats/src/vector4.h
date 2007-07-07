#ifndef VECTOR4_H
#define VECTOR4_H

#include <iostream>
#include <cmath>
#include <cassert>
#include <limits>

template<class T>
class Vector4
{
private:
  T x, y, z, w;

public:
  static const T epsilon;

public:
  Vector4(): x(0), y(0), z(0), w(0) {}
  Vector4(T inX, T inY, T inZ, T w): x(inX), y(inY), z(inZ), w(w) {}
  Vector4(const Vector4<T>& v): x(v.x), y(v.y), z(v.z), w(v.w) {}

  void getValue(T &x, T &y, T &z, T& w) const {
    x = this->x; y = this->y; z = this->z;
    w = this->w;
  }

  void setValues(T x, T y, T z, T w) {
    this->x = x; this->y = y; this->z = z;
    this->w = w;
  }

  void setValues(const Vector4<T>& v) {
    x = v.x; y = v.y; z = v.z;
    w = v.w;
  }

  void setValues(const T v[4]) {
    x = v[0]; y = v[1]; z = v[2];
    w = v[3];
  }

  // If everything is negatif, OpenSG returns epsilon
  T maxValue() {
    T m = (x>epsilon)?x:epsilon;
    m = (y>m)?y:m;
    m = (m>z)?m:z;
    return std::max(m, w);
  }

  T& operator[](unsigned int i) {
    assert(i<4);
    return *(&x+i);
  }

  const T &operator[](unsigned int i) const {
    assert(i<4);
    return *(&x+i);
  }

  Vector4<T> operator-() const {
    return Vector4<T>(-x, -y, -z, -w);
  }

  Vector4<T>& operator=(const Vector4<T> &v) {
    x = v.x; y = v.y; z = v.z; w = v.w;
    return *this;
  }

  Vector4<T>& operator+=(const Vector4<T> &v) {
    x += v.x; y += v.y; z += v.z; w += v.w;
    return *this;
  }

  Vector4<T>& operator-=(const Vector4<T>& v)  {
    x -= v.x; y -= v.y; z -= v.z; w -= v.w;
    return *this;
  }

  Vector4<T>& operator*=(T k) {
    x *= k; y *= k; z *= k; w *= k;
    return *this;
  }

  Vector4<T>& operator/=(T k)  {
    assert(k != 0);
    x /= k; y /= k; z /= k; w /= k;
    return *this;
  }

  Vector4<T> operator+(const Vector4<T> &v) const {
    return Vector4<T>(x+v.x, y+v.y, z+v.z, w+v.w);
  }

  Vector4<T> operator-(const Vector4<T> &v) const {
    return Vector4<T>(x-v.x, y-v.y, z-v.z, w-v.w);
  }

  T operator*(const Vector4<T> &v) const {
    return dot(v);
  }

  Vector4<T> operator*(T k) const {
    return Vector4<T>(x*k, y*k, z*k, w*k);
  }

  Vector4<T> operator/(T k) const {
    assert(k!=0);
    return Vector4<T>(x/k, y/k, z/k, w/k);
  }

  bool operator==(const Vector4<T> &v) const {
    using namespace std;
    return abs(x-v.x)<epsilon && abs(y-v.y)<epsilon && abs(z-v.z)<epsilon && abs(w-v.w)<epsilon;
  }

  bool isNull() const {
    return *this == Vector4<T>(0,0,0,0);
  }

  T dist(const Vector4<T> &v) const {
    return (*this - v).length();
  }

  T dist2(const Vector4<T> &v) const {
    return (*this - v).length2();
  }

  T length() const {
    return std::sqrt(x*x+y*y+z*z+w*w);
  }

  T length2() const {
    return x*x+y*y+z*z+w*w;
  }

  void normalize() {
    T l = length();
    if(abs(l) < epsilon)
      x=y=z=w=0;
    else
      *this *= 1.0f/l;
  }

  T dot(const Vector4<T> &v) const {
    return x*v.x+y*v.y+z*v.z+w*v.w;
  }

  friend Vector4<T> operator*(T k, const Vector4<T>&v) {
    return v*k;
  }

  friend std::ostream& operator<<(std::ostream& os, const Vector4<T>& v) {
    return os << v.x << ", " << v.y << ", " << v.z << ", " << v.w;
  }

};

template<class T>
const T Vector4<T>::epsilon = std::numeric_limits<T>::epsilon();

typedef Vector4<float> Vec4f;

#endif
