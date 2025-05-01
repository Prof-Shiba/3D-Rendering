#include "./display.h"

bool is_running = false;

void setup(void) {
  color_buffer =
      (uint32_t *)malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);

  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           WINDOW_WIDTH, WINDOW_HEIGHT);
}

void update(void) {
  // TODO:
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

void render(void) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderClear(renderer);

  render_color_buffer();
  clear_color_buffer(0xFF000000);
  draw_grid();
  draw_rectangle(800, 800, 300, 50, 0xFFFFFF00);

  // double buffer
  SDL_RenderPresent(renderer);
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
