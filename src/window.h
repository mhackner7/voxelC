#pragma once 
#include "gfx_libs.h"
#include "types.h"

typedef struct Window {
    SDL_Window *sdl_handle;
    uint16_t    width;
    uint16_t    height;
    bool        show_wireframe;
} Window;


void init_window(Window *window);

void end_window(Window *window);

f32 get_aspect_ratio(Window *window);

// .c file, for lazy compiling 

