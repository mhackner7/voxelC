#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"
#include "gfx_libs.h"

#include "../include/fastnoise.h"
#define NOISE_FRQ 0.001f

#define CHUNK_LENGTH 16
#define CHUNK_HEIGHT 256

//each chunk is about 0.131mb
typedef struct {
    Block       blocks[CHUNK_LENGTH][CHUNK_HEIGHT][CHUNK_LENGTH];
    metadata    meta[CHUNK_LENGTH][CHUNK_HEIGHT][CHUNK_LENGTH];
    bool        dirty;
    s16         x, z;
    GLuint      VAO, VBO, UVBO, NVBO, EBO;
    u32         index_count;
} Chunk;

#define CHUNK_CACHE_DISTANCE  30
#define CHUNK_RENDER_DISTANCE 20


/*
worlds are NOT infinite

later i may revisit and make the worlds infinite, 
but for now, they will have a fixed 64 x 64 chunk size ( 1024 x 1024 blocks )

*/

#define WORLD_CHUNK_LIMIT 32



/*

    everything deals in world coordinates
    chunks are 1 x 1 world coord unit
    player position is in actual coordinates

*/

#define SAVE_NAME_LIMIT 32

typedef struct World {
    Chunk       chunks[WORLD_CHUNK_LIMIT][WORLD_CHUNK_LIMIT];
    char        save_name[SAVE_NAME_LIMIT];   
    fnl_state   state;
} World;

void init_chunk(Chunk *chunk, u16 x, u16 z, fnl_state* noise_state);

void render_chunk(Chunk *chunk);

void clean_chunk(Chunk *chunk, World *world);

void free_chunk(Chunk *chunk);





#endif