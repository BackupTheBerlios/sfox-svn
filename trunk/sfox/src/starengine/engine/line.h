#ifndef LINE_H
#define LINE_H

#include "bbox.h"
#include "mathengine.h"

enum {LFRONT=0, LBACK, LCOLLINEAR, LINTERSECT};

typedef struct {
  vector2 normal;
  float d;
} line2d;

extern void line2d_set(line2d *l, float nx, float ny, float d);
extern int line2d_test_point(line2d *l, float x, float y);
extern int line2d_test_vector2(line2d *l, vector2 *v);
extern int line2d_test_polygon(line2d *l, vector2 *point, unsigned int num);
extern int line2d_test_bbox(line2d *l, bbox2d *bbox);

#endif
