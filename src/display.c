#include "./display.h"

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

void render_color_buffer(void) {
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer,
                    (uint32_t)(WINDOW_WIDTH * sizeof(uint32_t)));

  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void draw_grid(void) {
  for (size_t row = 0; row < WINDOW_HEIGHT; row += 10) {
    for (size_t col = 0; col < WINDOW_WIDTH; col += 10)
      color_buffer[(WINDOW_WIDTH * row) + col] = 0xFF555555;
  }
}

void draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
  if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
    color_buffer[(WINDOW_WIDTH * y) + x] = color;
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

void clear_color_buffer(uint32_t color) {
  for (size_t row = 0; row < WINDOW_HEIGHT; row++) {
    for (size_t col = 0; col < WINDOW_WIDTH; col++) {
      color_buffer[(WINDOW_WIDTH * row) + col] = color;
    }
  }
}

void destroy_window(void) {
  free(color_buffer);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}
