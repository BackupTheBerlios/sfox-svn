#ifndef VERTEX_H
#define VERTEX_H

struct Vertex {
  float x, y, z;
  float nx, ny, nz;
  float u, v;
  float tangent[3];
};

#define OFFSETCOORD 0
#define OFFSETNORM (3*sizeof(float))
#define OFFSETTCOORD (OFFSETNORM+3*sizeof(float))
#define OFFSETTANGENT (OFFSETTCOORD+2*sizeof(float))

#endif
