#include "./matrix.h"

mat4_t mat4_identity(void) {
  mat4_t m = {
    { 1, 0, 0, 0},
    { 0, 1, 0, 0},
    { 0, 0, 1, 0},
    { 0, 0, 0, 1}
  };
  return m;
}

mat4_t mat4_scale(float scale_x, float scale_y, float scale_z) {
  // | sx 0 0 0 |
  // | 0 sy 0 0 |
  // | 0 0 sz 0 |
  // | 0 0 0  1 |
  mat4_t m = mat4_identity();

  m.m[0][0] = scale_x;
  m.m[1][1] = scale_y;
  m.m[2][2] = scale_z;

  return m;
}
