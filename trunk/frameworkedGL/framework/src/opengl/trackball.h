#ifndef TRACKBALL_H
#define TRACKBALL_H

#include "math/quaternion.h"

namespace StarEngine {
  class Trackball {
  private:
    static const float INVSQRT2;
    int w, h;
    bool dragging;

    Quaternionf orientation, startOrientation;
    Vec3f startVector, endVector;
    void mapToSphere(Vec3f &v);

  public:
    Trackball(int w,  int h):w( w ), h( h ), dragging( false ) {
      orientation.toIdentity();
      startOrientation.toIdentity();
    }

    void startRotation(int x, int y);
    void stopRotation();
    void computeOrientation(int x, int y);
    const Quaternionf &getOrientation() { return orientation; }
    void toOpenGL() const;
  };
}
#endif
