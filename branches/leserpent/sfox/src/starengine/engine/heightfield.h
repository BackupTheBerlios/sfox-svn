#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#include "object3d.h"

typedef struct heightfield *heightfield;

/*image size must be (2^n)+1 x (2^m)+1*/
extern heightfield heightfield_create_from_file(char *heightmap_filename,
                                                double sizex, double sizey,
                                                double sizez);

extern void heightfield_set_textures_from_file(heightfield hf, char *land, char *details);
extern void heightfield_set_detail_scale(heightfield hf, double sx, double sy);

extern void heightfield_to_opengl(heightfield hf);

/*Common setters and getters*/
extern object3d heightfield_get_object(heightfield hf);
extern void heightfield_set_local_matrix(heightfield hf, matrix4 local);
extern void heightfield_set_world_matrix(heightfield hf, matrix4 world);
extern matrix4 *heightfield_get_world_matrix(heightfield hf);
extern matrix4 *heightfield_get_local_matrix(heightfield hf);

#endif
