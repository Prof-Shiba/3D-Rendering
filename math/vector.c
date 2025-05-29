#include "./vector.h"
#include <math.h>

void vec2_normalize(vec2_t *v) {
  float length = vec2_length(*v);
  v->x /= length;
  v->y /= length;
}

float vec2_length(vec2_t v) { return sqrt((v.x * v.x) + (v.y * v.y)); }
float vec2_dot(vec2_t a, vec2_t b) { return (a.x * b.x) + (a.y * b.y); }

vec2_t vec2_add(vec2_t a, vec2_t b) {
  vec2_t new_vec = {a.x + b.x, a.y + b.y};
  return new_vec;
}

vec2_t vec2_sub(vec2_t a, vec2_t b) {
  vec2_t new_vec = {a.x - b.x, a.y - b.y};
  return new_vec;
}

vec2_t vec2_mul(vec2_t v, float scalar) {
  vec2_t new_vec = {v.x * scalar, v.y * scalar};
  return new_vec;
}

vec2_t vec2_div(vec2_t v, float scalar) {
  vec2_t new_vec = {v.x / scalar, v.y / scalar};
  return new_vec;
}

void vec3_normalize(vec3_t *v) {
  float length = vec3_length(*v);
  v->x /= length;
  v->y /= length;
  v->z /= length;
}

float vec3_length(vec3_t v) {
  return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

float vec3_dot(vec3_t a, vec3_t b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
  vec3_t new_vec = {a.x + b.x, a.y + b.y, a.z + b.z};
  return new_vec;
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
  vec3_t new_vec = {a.x - b.x, a.y - b.y, a.z - b.z};
  return new_vec;
}

vec3_t vec3_mul(vec3_t v, float scalar) {
  vec3_t new_vec = {v.x * scalar, v.y * scalar, v.z * scalar};
  return new_vec;
}

vec3_t vec3_div(vec3_t v, float scalar) {
  vec3_t new_vec = {v.x / scalar, v.y / scalar, v.z / scalar};
  return new_vec;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
  vec3_t new_vec = {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                    a.x * b.y - a.y * b.x};
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

vec4_t vec4_from_vec3(vec3_t v) {
  vec4_t result = {v.x, v.y, v.z, 1.0};
  return result;
}

vec3_t vec3_from_vec4(vec4_t v) {
  vec3_t result = {v.x, v.y, v.z};
  return result;
}
