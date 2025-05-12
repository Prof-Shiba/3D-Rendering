#pragma once
#include "./triangle.h"
#include "./vector.h"

#define NUM_CUBE_VERTICES 8
#define NUM_CUBE_FACES (6 * 2) // 6 faces, 2 triangles per face

extern vec3_t cube_vertices[NUM_CUBE_VERTICES];
extern face_t cube_faces[NUM_CUBE_FACES];

typedef struct {
  vec3_t *vertices;
  face_t *faces;
  vec3_t rotation;
} mesh_t;

extern void load_cube_mesh_data(void);
extern mesh_t mesh;
extern void load_obj_data(char *file_name);
