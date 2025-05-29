#pragma once

// scale, translation, rotation
// translation requires 4x4, not 3x3
// x, y, z, w; w->1 (initially)
typedef struct {
  float m[4][4];
} mat4_t;

mat4_t mat4_identity(void);
mat4_t mat4_scale(float scale_x, float scale_y, float scale_z);
