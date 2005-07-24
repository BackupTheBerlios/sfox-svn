#include "trackball.h"
#include <GL/gl.h>

namespace StarEngine {
  void
  Trackball::mapToSphere(Vec3f &v)
  {
    float len2;

    v.x = (2*v.x)/float(w-1)-1;
    v.y = 1-(2*v.y)/float(h-1);
    v.z = 0;
    if((len2 = v.length2())<INVSQRT2)
      v.z = std::sqrt(1.0-len2); // We are on the sphere
    else
      v.z = 1.0f/(2*std::sqrt(len2)); // On the hyperbole
  }

  void
  Trackball::computeOrientation(int x, int y)
  {
    if ( !dragging )
      return;
    Quaternionf q;
    Vec3f endVector(x, y, 0);

    mapToSphere(endVector);
    q.toRotationArc(startVector, endVector);
    orientation = q*startOrientation;
    orientation.normalize();
  }

  void
  Trackball::startRotation(int x, int y)
  {
    startVector.setValues(x, y, 0);
    mapToSphere(startVector);
    dragging = true;
  }

  void
  Trackball::stopRotation()
  {
    startOrientation = orientation;
    dragging = false;
  }

  void
  Trackball::toOpenGL() const
  {
    //float m[16];
    float m[4][4];
    orientation.unitToMatrix44(m);
    glMultMatrixf((float * )m);
  }

  const float Trackball::INVSQRT2=1.0f/std::sqrt(2.0f);
}
