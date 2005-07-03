#ifndef QUATERION_H
#define QUATERION_H

#include <iostream>
#include <cmath>
#include <cassert>
#include <limits>

#include "vector3.h"
#include "matrix.h"

template<class T>
class Quaternion {
public:
  T x, y, z, w;

  Quaternion() {}
  Quaternion(T x, T y, T z, T w): x(x), y(y), z(z), w(w) {}

  void setValues(T x, T y, T z, T w) {
    this->x = x; this->y  = y; this->z = z;
    this->w = w;
  }
  
  Quaternion<T> operator*(const Quaternion<T> &r) const {
    Quaternion<T> res;

    res.w = w*r.w - x*r.x - y*r.y - z*r.z;
    res.x = w*r.x + x*r.w + y*r.z - z*r.y;
    res.y = w*r.y - x*r.z + y*r.w + z*r.x;
    res.z = w*r.z + x*r.y - y*r.x + z*r.w;
   
    return res;
  }

  T norm() const {
    return std::sqrt(norm2());
  }

  T norm2() const {
    return x*x+y*y+z*z+w*w;
  }

  void normalize() {
    T k = T(1)/norm();
    x *= k; y *= k; z *= k;
    w *= k;
  }

  void toIdentity() {
    setValues(0, 0, 0, 1);
  }

  void toConjugate() {
    x = -x;
    y = -y;
    z = -z;
    w = w;
  }

  Quaternion<T> getConjugate() const {
    return Quaternion<T>(-x, -y, -z, w);
  }

  void getAxisAngle(float *ax, float *ay, float *az, float *theta) {
    float scale = std::sqrt(x*x+y*y+z*z);
    if(scale<Vector3<T>::epsilon) {
      *theta = 2*std::acos(w);
      *ax = x/scale;
      *ay = y/scale;
      *az = z/scale;
    } else {
      *theta = 0;
      *ax = 1;                    /* Identity  */
      *ay = 0;
      *az = 0;
    }
  }

  void unitToMatrix44(Matrix4<T> &m) const {
    float x2=x*x;
    float y2=y*y;
    float z2=z*z;

    m[0][0] = 1 - 2*y2 - 2*z2;
    m[0][1] = 2*x*y - 2*w*z;
    m[0][2] = 2*x*z + 2*w*y;
    m[1][0] = 2*x*y + 2*w*z;
    m[1][1] = 1 - 2*x2 - 2*z2;
    m[1][2] = 2*y*z - 2*w*x;
    m[2][0] = 2*x*z - 2*w*y;
    m[2][1] = 2*y*z + 2*w*x;
    m[2][2] = 1 - 2*x2 - 2*y2;
    m[3][0] = m[3][1] = m[3][2] = m[0][3] = m[1][3] = m[2][3] = 0;
    m[3][3] = 1;
  }

  // Reference: Stan Melax, Game Programming Gems
  void toRotationArc(Vector3<T> &u, Vector3<T> &v) {
    assert(!(u.isNull() && v.isNull()));
    u.normalize();
    v.normalize();
    Vector3<T> w = u.cross(v);
    
    T d = u*v;
    T s = std::sqrt((1+d)*2);
    setValues(w.x/s, w.y/s, w.z/s, s/T(2));
  }

  void rotateVector(Vector3<T> &v) const {
    Quaternion<T> tmp(v.x, v.y, v.z, 1);
    tmp = *this*tmp*getConjugate();
    v.setValues(tmp.x, tmp.y, tmp.z);
  }

  void fromAxisAngle(const Vector3<T> &v, T theta) {
    using namespace std;
    if(!v.isNull()) {
      theta /= T(2);
      T s = sin(theta);
      T c = cos(theta);
      x = v.x*s; y = v.y*s;
      z = v.z*s; w = c;
    } else {
      x = y = z = 0;     /* Axis is vector 0 so identity quaternion */
      w = 1.0;
    }
  }

  
  friend std::ostream& operator<<(std::ostream& os, const Quaternion<T>& q) {
    return os << "(" << q.x << ", " << q.y << ", " << q.z << "), " << q.w;
  }
};

typedef Quaternion<float> Quaternionf;

#endif
