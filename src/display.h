#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

// Variables and global declaration
extern SDL_Window* window;
extern SDL_Renderer* renderer;
// uint32_t instead of int to make sure it's a 32 bit integer
extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;
// here using int because we don't care about the size of int as it's only 800/600
extern int window_width;
extern int window_height;

bool initialize_window(void);
void draw_grid(void);
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void destroy_window(void);

#endif
