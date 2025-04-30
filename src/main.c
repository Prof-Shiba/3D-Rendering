#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>

#define DEFAULT_MONITOR_NUMBER -1

bool is_running = false;

uint16_t WINDOW_WIDTH = 2560;
uint16_t WINDOW_HEIGHT = 1440;

uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool init_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "SDL failed to initialize!\n");
    return false;
  }

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

  return true;
}

void setup(void) {
  color_buffer =
      (uint32_t *)malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);

  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           WINDOW_WIDTH, WINDOW_HEIGHT);
}

void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT:
    is_running = false;
    break;

  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE) {
      is_running = false;
      break;
    }
  }
}

void update(void) {
  // TODO:
}

void render_color_buffer(void) {
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer,
                    (uint32_t)(WINDOW_WIDTH * sizeof(uint32_t)));

  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
  for (size_t row = 0; row < WINDOW_HEIGHT; row++) {
    for (size_t col = 0; col < WINDOW_WIDTH; col++) {
      color_buffer[(WINDOW_WIDTH * row) + col] = color;
    }
  }
}

void render(void) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderClear(renderer);

  render_color_buffer();
  clear_color_buffer(0xFFFFFF00);

  // double buffer
  SDL_RenderPresent(renderer);
}

void destroy_window(void) {
  free(color_buffer);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

int main(void) {
  is_running = init_window();

  setup();

  while (is_running) {
    process_input();
    update();
    render();
  }

  destroy_window();
}
