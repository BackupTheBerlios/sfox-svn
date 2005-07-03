#ifndef MATRIX_H
#define MATRIX_H

#include <cassert>

#include "vector3.h"

template<class T>
class Matrix4 {
private:
  T m[4][4];
  
public:
  Matrix4() {}
  ~Matrix4() {}

  void toIdentity() {
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
    m[0][1] = m[0][2] = m[0][3] = 0;
    m[1][0] = m[1][2] = m[1][3] = 0;
    m[2][0] = m[2][1] = m[2][3] = 0;
    m[3][0] = m[3][1] = m[3][2] = 0;
  }

  void toScale(float sx, float sy, float sz) {
    toIdentity();
    m[0][0] = sx; m[1][1] = sy; m[2][2] = sz;
  }
  
  T *operator[](unsigned int i) {
    assert (i<4);
    return m[i];
  }

  Vector3<T> getCol(int n) {
    assert(n<4);
    return Vector3<T>(m[0][n], m[1][n], m[2][n]);
  }

  friend std::ostream& operator<<(std::ostream& os, Matrix4<T>& m) {
    return os << m[0][0] << " " << m[1][0] << " " << m[2][0] << " "<< m[3][0]<< "\n"
              << m[0][1] << " " << m[1][1] << " " << m[2][1] << " "<< m[3][1] << "\n"
              << m[0][2] << " " << m[1][2] << " " << m[2][2] << " "<< m[3][2] << "\n"
              << m[0][3] << " " << m[1][3] << " " << m[2][3] << " "<< m[3][3] << "\n";
  }

};

typedef Matrix4<float> Matrix4f;


#endif
