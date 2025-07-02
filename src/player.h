#pragma once

#include "camera.h"
#include "input.h"

typedef enum {
    access_lvl0, // does not have any permissions
    access_lvl1, // has view-only permissions
    access_lvl2, // can break/place blocks
    access_lvl3, // has full access to world settings and player state
} access_e;

typedef enum {
    none,
    adventure,
    survival,
    creative,
} pl_state_e;

typedef struct {
    Camera      cam;
    u64         x, y, z;
    f32         height;
    f32         width;
    f32         depth;
    bool        win_focused;
    access_e    clearence;
    pl_state_e  state;
} Player;

Player init_player(u64 x, u64 y, u64 z);

void update_player(Player *player, Window *window, SDL_Event *event, GLuint shader, f64 delta_time);
