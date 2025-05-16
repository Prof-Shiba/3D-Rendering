#include "./vector.h"
#include <math.h>

float vec2_length(vec2_t v) { return sqrt((v.x * v.x) + (v.y * v.y)); }

vec2_t vec2_add(vec2_t a, vec2_t b) {
  vec2_t new_vec = {a.x + b.x, a.y + b.y};
  return new_vec;
}

vec2_t vec2_sub(vec2_t a, vec2_t b) {
  vec2_t new_vec = {a.x - b.x, a.y - b.y};
  return new_vec;
}

float vec3_length(vec3_t v) {
  return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
  vec3_t new_vec = {a.x + b.x, a.y + b.y, a.z + b.z};
  return new_vec;
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
  vec3_t new_vec = {a.x - b.x, a.y - b.y, a.z - b.z};
  return new_vec;
}

vec3_t vec3_rotate_x(vec3_t v, float angle) {
  vec3_t rotated_vector = {v.x, v.y * cos(angle) - v.z * sin(angle),
                           v.y * sin(angle) + v.z * cos(angle)};
  return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
  vec3_t rotated_vector = {v.x * cos(angle) - v.z * sin(angle), v.y,
                           v.x * sin(angle) + v.z * cos(angle)};
  return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle) {
  vec3_t rotated_vector = {v.x * cos(angle) - v.y * sin(angle),
                           v.x * sin(angle) + v.y * cos(angle), v.z};
  return rotated_vector;
}
