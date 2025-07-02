#pragma once
#include "types.h"

//#include "block.h"


typedef struct {

    unsigned int  handle;
    int           x, y;
    int           channels;
    int           tile_size;

} Atlas;

typedef struct {
    vec2 min;
    vec2 max;
} UVs;

Atlas load_atlas(const char *filename);


/** 
 * @brief
 * takes in an x/y coord for the texture atlas
 * based on which tile is desired
 *  --  1,1 would map to the bottom left of the texture atlas
 *  --  16, 16 would map to the top right
 * converts to uv coordinates stored in a UV struct (max and min)
 * min is bottom left of the tile, max is top right
 */
UVs tex_get_uvs(u16 x, u16 y);

//c file



