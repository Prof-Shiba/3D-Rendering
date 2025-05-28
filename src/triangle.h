#pragma once
#include "../math/vector.h"
#include <stdint.h>

typedef struct {
  uint32_t a;
  uint32_t b;
  uint32_t c;
} face_t;

typedef struct {
  vec2_t points[3];
  float avg_depth;
} triangle_t;

void draw_filled_triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                          uint16_t x2, uint16_t y2, uint32_t color);
