#pragma once
#include "./vector.h"
#include <stdint.h>

typedef struct {
  uint32_t a;
  uint32_t b;
  uint32_t c;
} face_t;

typedef struct {
  vec2_t points[3];
} triangle_t;
