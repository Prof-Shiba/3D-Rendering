#pragma once
#include "../math/vector.h"
#include <math.h>

// scale, translation, rotation
// translation requires 4x4, not 3x3
// x, y, z, w; w->1 (initially)
typedef struct {
  float m[4][4];
} mat4_t;

mat4_t mat4_identity(void);
mat4_t mat4_scale(float scale_x, float scale_y, float scale_z);
vec4_t mat4_mul_vec4(mat4_t m, vec4_t v);
mat4_t mat4_translation(float translation_x, float translation_y, float translation_z);
mat4_t mat4_rotation_x(float angle);
mat4_t mat4_rotation_y(float angle);
mat4_t mat4_rotation_z(float angle);
mat4_t mat4_mul_mat4(mat4_t lhs, mat4_t rhs);
mat4_t mat4_perspective(float fov, float aspect, float z_near, float z_far);
vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v);
