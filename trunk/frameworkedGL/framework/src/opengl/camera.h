#ifndef CAMERA_H
#define CAMERA_H

#include "math/quaternion.h"
#include "math/matrix.h"

namespace StarEngine {
  class Camera {
  public:
    Camera(float fov, float znear, float zfar);

    void toOpenGL();

    /*Interactive motion*/
    void mouseMove(float xrel, float yrel);
    void moveAlongView(float speed);
    void sideMove(float speed);

    /*Setters*/
    void setPos(float x, float y, float z);
    void setUp(float x, float y, float z);
    void setMaxPitch(float t) { maxPitch = t; }
    void setMaxYaw(float t) { maxYaw = t; }
    enum {YPRNOLIMIT=-1};

  private:
    Vec3f up, pos;
    Quaternionf orientation;
    Matrix4f viewMatrix;

    float fov, znear, zfar;
    float pitch, yaw, roll;
    float maxPitch, maxYaw;
    unsigned int update;

    enum {
      UPDATE_PERSPECTIVE = 1 << 0,
      UPDATE_VIEW = 1 << 1
    };

    void setPerspective();
    void setView();
  };
}
#endif
