#include "./display.h"
#include "./vector.h"

// NOTE: cube will go from -1 to 1, 0 is in the center, need 9 points to
// represent that, inc by .25
#define NUM_OF_POINTS (9 * 9 * 9)

bool is_running = false;
float fov_factor = 128;

vec3_t cube_points[NUM_OF_POINTS];
vec2_t projected_points[NUM_OF_POINTS];
void setup(void) {
  color_buffer =
      (uint32_t *)malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);

  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           WINDOW_WIDTH, WINDOW_HEIGHT);
  uint32_t point_count = 0;
  for (float x = -1; x <= 1; x += 0.25) {
    for (float y = -1; y <= 1; y += 0.25) {
      for (float z = -1; z <= 1; z += 0.25) {
        vec3_t new_point = {x, y, z};
        cube_points[point_count++] = new_point;
      }
    }
  }
}

vec2_t project(vec3_t point) {
  vec2_t projected_point = {
      (fov_factor * point.x),
      (fov_factor * point.y),
  };
  return projected_point;
}

void update(void) {
  for (size_t i = 0; i < NUM_OF_POINTS; i++) {
    vec3_t point = cube_points[i];
    vec2_t projected_point = project(point);

    projected_points[i] = projected_point;
  }
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
  draw_grid();

  for (size_t i = 0; i < NUM_OF_POINTS; i++) {
    vec2_t projected_point = projected_points[i];
    draw_rectangle(projected_point.x + (uint16_t)(WINDOW_WIDTH / 2),
                   projected_point.y + (uint16_t)(WINDOW_HEIGHT / 2), 4, 4,
                   0xFFFF0000);
  }

  render_color_buffer();
  clear_color_buffer(0xFF000000);

  // double buffer
  SDL_RenderPresent(renderer);
}

int main(void) {
  is_running = init_window();

  setup();

  vec3_t vector = {2.0, 3.0, -4.0};

  while (is_running) {
    process_input();
    update();
    render();
  }

  destroy_window();
}
