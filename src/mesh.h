#pragma once
#include "./triangle.h"
#include "../math/vector.h"

typedef struct {
  vec3_t *vertices;
  face_t *faces;
  vec3_t rotation;
  vec3_t scale;
  vec3_t translation;
} mesh_t;

extern mesh_t mesh;
extern void load_obj_data(char *file_src);
