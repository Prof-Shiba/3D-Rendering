#pragma once
#include "./triangle.h"
#include "./vector.h"

#define NUM_MESH_VERTICES 8
extern vec3_t mesh_vertices[NUM_MESH_VERTICES];

#define NUM_MESH_FACES (6 * 2) // 6 faces, 2 triangles per face
extern face_t mesh_faces[NUM_MESH_FACES];
