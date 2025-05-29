#pragma once
#include "../math/vector.h"

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
