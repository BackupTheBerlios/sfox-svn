#ifndef FRUSTUM_H
#define FRUSTUM_H

#ifdef _WIN32
#undef near
#undef far
#endif

#include "bbox.h"
#include "line.h"
#include "plan.h"

/* all plane faces inward */
typedef struct {
  plane far;
  plane near;
  plane left;
  plane right;
  plane top;
  plane bottom;
} frustum;

/* all line faces inward */
typedef struct {
  line2d left;
  line2d right;
  line2d front;
  line2d back;
} frustum2d;

extern void frustum_get_from_opengl(frustum *ftm);
extern int frustum_point_is_into(frustum *ftm, float x, float y, float z);
extern void frustum_copy(frustum *dst, frustum *src);
/*Only ftm is modified*/
extern void frustum_transform(frustum *ftm, matrix4 m, vector3 *pos);
extern void frustum_transform_and_copy(frustum *dst, frustum *ftm, matrix4 m, vector3 *pos);
extern void frustum_print(frustum *ftm);


/*Project the frustum on the xy plane*/
extern void frustum_to_frustum2d_xz(frustum2d *ftm2d, frustum *ftm);
extern int frustum2d_point_is_into(frustum2d *ftm, float x, float y);
extern int frustum2d_bbox_is_into(frustum2d *ftm2d, bbox2d *bb);

#endif
