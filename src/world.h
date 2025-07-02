#pragma once

#include "chunk.h"
#include "player.h"

World *create_world();

World *init_world(char *save_name, bool is_new, u32 seed);

void render_world(World *world);

void save_world(World *world);

void sync_world(World *world);

