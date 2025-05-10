#pragma once
#include "./triangle.h"
#include "./vector.h"

#define NUM_CUBE_VERTICES 8
extern vec3_t cube_vertices[NUM_CUBE_VERTICES];

#define NUM_CUBE_FACES (6 * 2) // 6 faces, 2 triangles per face
extern face_t cube_faces[NUM_CUBE_FACES];
