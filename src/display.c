#include "./display.h"
#include <stdlib.h>

uint16_t WINDOW_WIDTH = 0;
uint16_t WINDOW_HEIGHT = 0;

uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool init_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "SDL failed to initialize!\n");
    return false;
  }

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode); // 0 is main monitor

  WINDOW_WIDTH = display_mode.w;
  WINDOW_HEIGHT = display_mode.h;

  window =
      SDL_CreateWindow("3D Renderer", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  if (!window) {
    fprintf(stderr, "SDL window failed to initialize!\n");
    return false;
  }

  renderer = SDL_CreateRenderer(window, DEFAULT_MONITOR_NUMBER, 0);
  if (!renderer) {
    fprintf(stderr, "SDL renderer failed to initialize!\n");
    return false;
  }

  if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0) {
    fprintf(stderr, "SDL failed to make the window fullscreen!\n");
    return false;
  }
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

  return true;
}

void clear_color_buffer(uint32_t color) {
  for (size_t row = 0; row < WINDOW_HEIGHT; row++) {
    for (size_t col = 0; col < WINDOW_WIDTH; col++) {
      color_buffer[(WINDOW_WIDTH * row) + col] = color;
    }
  }
}

void render_color_buffer(void) {
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer,
                    (uint32_t)(WINDOW_WIDTH * sizeof(uint32_t)));

  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
    color_buffer[(WINDOW_WIDTH * y) + x] = color;
}

void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
               uint32_t color) {
  int16_t delta_x = (x1 - x0);
  int16_t delta_y = (y1 - y0);

  uint16_t longest_side_length =
      (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

  float x_inc = delta_x / (float)longest_side_length;
  float y_inc = delta_y / (float)longest_side_length;

  float current_x = x0;
  float current_y = y0;

  for (size_t i = 0; i <= longest_side_length; i++) {
    draw_pixel(round(current_x), round(current_y), color);
    current_x += x_inc;
    current_y += y_inc;
  }
}

void draw_grid(void) {
  for (size_t row = 0; row < WINDOW_HEIGHT; row += 10) {
    for (size_t col = 0; col < WINDOW_WIDTH; col += 10)
      color_buffer[(WINDOW_WIDTH * row) + col] = 0xFF333333;
  }
}

void draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                    uint32_t color) {
  for (size_t row = 0; row < height; row++) {
    for (size_t col = 0; col < width; col++) {
      uint32_t new_x = x + row;
      uint32_t new_y = y + col;
      draw_pixel(new_x, new_y, color);
    }
  }
}

void draw_triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                   uint16_t x2, uint16_t y2, uint32_t color) {
  draw_line(x0, y0, x1, y1, color);
  draw_line(x1, y1, x2, y2, color);
  draw_line(x2, y2, x0, y0, color);
}

void destroy_window(void) {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}
