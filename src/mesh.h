#pragma once
#include "./triangle.h"
#include "./vector.h"

typedef struct {
  vec3_t *vertices;
  face_t *faces;
  vec3_t rotation;
} mesh_t;

extern void load_cube_mesh_data(void);
extern mesh_t mesh;
extern void load_obj_data(char *file_src);
