#include "./triangle.h"
#include "display.h"
#include <stddef.h>

void int_swap(uint16_t *a, uint16_t *b) {
  uint16_t temp = *a;
  *a = *b;
  *b = temp;
}

void fill_bottom_tri(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                     uint16_t x2, uint16_t y2, uint32_t color) {
  // inverse slope
  float inv_slope_1 = (float)(x1 - x0) / (y1 - y0);
  float inv_slope_2 = (float)(x2 - x0) / (y2 - y0);

  float x_start = x0;
  float x_end = x0;

  for (size_t y = y0; y <= y2; y++) {
    draw_line(x_start, y, x_end, y, color);
    x_start += inv_slope_1;
    x_end += inv_slope_2;
  }
}

void fill_top_tri(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                  uint16_t x2, uint16_t y2, uint32_t color) {
  float inv_slope_1 = (float)(x2 - x0) / (y2 - y0);
  float inv_slope_2 = (float)(x2 - x1) / (y2 - y1);

  float x_start = x2;
  float x_end = x2;

  for (size_t y = y2; y >= y0; y--) {
    draw_line(x_start, y, x_end, y, color);
    x_start -= inv_slope_1;
    x_end -= inv_slope_2;
  }
}

// draws a filled triangle using flat-top/flat-bottom method
// splits tri in 2, (bottom and top), then fills each portion
void draw_filled_triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                          uint16_t x2, uint16_t y2, uint32_t color) {
  if (y0 > y1) {
    int_swap(&y0, &y1);
    int_swap(&x0, &x1);
  }
  if (y1 > y2) {
    int_swap(&y1, &y2);
    int_swap(&x1, &x2);
  }
  if (y0 > y1) {
    int_swap(&y0, &y1);
    int_swap(&x0, &x1);
  }

  uint16_t My = y1;
  uint16_t Mx = ((x2 - x0) * (y1 - y0)) / (y2 - y0) + x0;

  fill_bottom_tri(x0, y0, x1, y1, Mx, My, color);
  fill_top_tri(x1, y1, Mx, My, x2, y2, color);
}
