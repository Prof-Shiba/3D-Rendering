#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>

#define DEFAULT_MONITOR_NUMBER -1
#define TARGET_FPS 60
#define FRAME_TARGET_TIME (1000 / TARGET_FPS)

extern uint16_t WINDOW_WIDTH;
extern uint16_t WINDOW_HEIGHT;

extern uint32_t *color_buffer;
extern SDL_Texture *color_buffer_texture;

extern SDL_Window *window;
extern SDL_Renderer *renderer;

bool init_window(void);
void destroy_window(void);

void render_color_buffer(void);
void clear_color_buffer(uint32_t color);

void draw_pixel(uint16_t x, uint16_t y, uint32_t color);
void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
               uint32_t color);
void draw_grid(void);
void draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                    uint32_t color);
