#include "./matrix.h"

mat4_t mat4_identity(void) {
  mat4_t m = {{
    { 1, 0, 0, 0},
    { 0, 1, 0, 0},
    { 0, 0, 1, 0},
    { 0, 0, 0, 1}
  }};
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

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v) {
  vec4_t result;

  result.x = (m.m[0][0] * v.x) + (m.m[0][1] * v.y) + (m.m[0][2] * v.z) + (m.m[0][3] * v.w);
  result.y = (m.m[1][0] * v.x) + (m.m[1][1] * v.y) + (m.m[1][2] * v.z) + (m.m[1][3] * v.w);
  result.z = (m.m[2][0] * v.x) + (m.m[2][1] * v.y) + (m.m[2][2] * v.z) + (m.m[2][3] * v.w);
  result.w = (m.m[3][0] * v.x) + (m.m[3][1] * v.y) + (m.m[3][2] * v.z) + (m.m[3][3] * v.w);

  return result;
}

mat4_t mat4_translation(float translation_x, float translation_y, float translation_z) {
  // | x + tx |
  // | y + ty |
  // | z + tz |
  // |   1    |
  mat4_t m = mat4_identity();

  m.m[0][3] = translation_x;
  m.m[1][3] = translation_y;
  m.m[2][3] = translation_z;

  return m;
}

mat4_t mat4_rotation_x(float angle) {
  float cosine = cos(angle);
  float sine = sin(angle);
  // | 1 0 0 0  |
  // | 0 c -s 0 |
  // | 0 s c 0  |
  // | 0 0 0 1  |

  mat4_t m = mat4_identity();
  m.m[1][1] = cosine;
  m.m[1][2] = -sine;
  m.m[2][1] = sine;
  m.m[2][2] = cosine;
  return m;
}

mat4_t mat4_rotation_y(float angle) {
  float cosine = cos(angle);
  float sine = sin(angle);
  // | c 0 s 0  |
  // | 0 1 0 0  |
  // | -s 0 c 0 |
  // | 0 0 0 1  |

  mat4_t m = mat4_identity();
  m.m[0][0] = cosine;
  m.m[0][2] = sine;
  m.m[2][0] = -sine;
  m.m[2][2] = cosine;
  return m;
}

mat4_t mat4_rotation_z(float angle) {
  float cosine = cos(angle);
  float sine = sin(angle);
  // | c -s 0 0 |
  // | s c 0 0  |
  // | 0 0 1 0  |
  // | 0 0 0 1  |

  mat4_t m = mat4_identity();
  m.m[0][0] = cosine;
  m.m[0][1] = -sine;
  m.m[1][0] = sine;
  m.m[1][1] = cosine;
  return m;
}

mat4_t mat4_mul_mat4(mat4_t lhs, mat4_t rhs) {
  mat4_t result;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result.m[i][j] = (lhs.m[i][0] * rhs.m[0][j]) + (lhs.m[i][1] * rhs.m[1][j]) + (lhs.m[i][2] * rhs.m[2][j]) + (lhs.m[i][3] * rhs.m[3][j]);
    }
  }
  return result;
}

mat4_t mat4_perspective(float fov, float aspect, float z_near, float z_far) {
  mat4_t m = {{{0}}};

  m.m[0][0] = aspect * (1 / tan(fov / 2));
  m.m[1][1] = 1 / tan(fov / 2);
  m.m[2][2] = z_far / (z_far / z_near);
  m.m[2][3] = (-z_far * z_near) / (z_far - z_near);
  m.m[3][2] = 1;
  return m;
}

vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v) {
  // mult proj matrix by our og vec
  vec4_t result = mat4_mul_vec4(mat_proj, v);

  // perform perspective divide with og z_value now stored in w
  if (result.w != 0) {
    result.x /= result.w;
    result.y /= result.w;
    result.z /= result.w;
  }
  return result;
}
