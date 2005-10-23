#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>

#include "camera.h"

#include "math/utils.h"

namespace StarEngine {
  Camera::Camera(float fov, float znear, float zfar)
    : fov(fov), znear(znear), zfar(zfar)
  {
    orientation.toIdentity();
    viewMatrix.toIdentity();
    setPos(0, 0, 0);
    setUp(0, 1, 0);
    update |= UPDATE_PERSPECTIVE;
  }

  void
  Camera::setPos(float x, float y, float z)
  {
    pos.setValues(x, y, z);
    update |= UPDATE_VIEW;
  }

  void
  Camera::setUp(float x, float y, float z)
  {
    up.setValues(x, y, z);
    update |= UPDATE_VIEW;
  }

/*****************************************************************************/
/* Interactive Motion                                                        */
/*****************************************************************************/
  void
  Camera::moveAlongView(float speed)
  {
    Vec3f view = viewMatrix.getCol(2);
    pos = pos-speed*view;
    update |= UPDATE_VIEW;
  }

  void
  Camera::sideMove(float speed)
  {
    Vec3f right = viewMatrix.getCol(0);
    pos = pos+(right*speed);

    update |= UPDATE_VIEW;
  }


  void
  Camera::mouseMove(float xrel, float yrel)
  {
    Quaternionf q;

    pitch = Utils::Deg360Bound(pitch+yrel);
    yaw = Utils::Deg360Bound(yaw+xrel);

    // if(pitch_not_max(cam, pitch)) {
    Vec3f tmp(1, 0, 0);
    orientation.rotateVector(tmp);
    q.fromAxisAngle(tmp, Utils::Deg2Rad(-yrel));
    orientation = q*orientation;
    //}

    //if(yaw_not_max(cam, yaw)) {
    q.fromAxisAngle(up, Utils::Deg2Rad(-xrel));
    orientation = q*orientation;
    //}
    update |= UPDATE_VIEW;
  }


/*****************************************************************************/
/* Matrices creation                                                         */
/*****************************************************************************/
  void
  Camera::toOpenGL()
  {
    setPerspective();
    setView();
  }

  void
  Camera::setView()
  {
    //if(update&UPDATE_VIEW) {
    orientation.unitToMatrix44(viewMatrix);
    viewMatrix[3][0] = -(viewMatrix[0][0]*pos.x+viewMatrix[1][0]*pos.y+
                         viewMatrix[2][0]*pos.z);
    viewMatrix[3][1] = -(viewMatrix[0][1]*pos.x+viewMatrix[1][1]*pos.y+
                         viewMatrix[2][1]*pos.z);
    viewMatrix[3][2] = -(viewMatrix[0][2]*pos.x+viewMatrix[1][2]*pos.y+
                         viewMatrix[2][2]*pos.z);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf((float *)&viewMatrix[0][0]);
    update &= ~UPDATE_VIEW;
    //}
  }

  void
  Camera::setPerspective()
  {
    //if(update&UPDATE_PERSPECTIVE) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (float)viewport[2]/(float)viewport[3], znear, zfar);
    glMatrixMode(GL_MODELVIEW);
    update &= ~UPDATE_PERSPECTIVE;
    //}
  }
}
