#include "chunk.h"

#define FNL_IMPL
#include "../include/fastnoise.h"



#include <stdlib.h>

#define MAX_BLOCKS           (CHUNK_LENGTH * CHUNK_HEIGHT * CHUNK_LENGTH)
#define MAX_VERTEX_FLOATS    (72 * MAX_BLOCKS)
#define MAX_INDICES          (36 * MAX_BLOCKS)
#define MAX_UVS              (24 * MAX_BLOCKS)
#define MAX_NORMALS          (72 * MAX_BLOCKS)

typedef struct {
    f32 *vertices;
    u32 *indices;
    f32 *uvs;
    f32 *normals;

    u32  vertex_size;
    u32  index_size;
    u32  uv_size;
    u32  normal_size;

    u32  vertex_count;
    u32  index_count;
    u32  uv_count;
    u32  normal_count;
} Mesh_builder;

static Mesh_builder *new_mesh_builder() {
    Mesh_builder *mb = malloc(sizeof(*mb));

    mb->vertex_size = MAX_VERTEX_FLOATS / 10;
    mb->index_size  = MAX_INDICES       / 10;
    mb->uv_size     = MAX_UVS           / 10;
    mb->normal_size = MAX_NORMALS       / 10;

    mb->vertex_count = 0;
    mb->index_count  = 0;
    mb->uv_count     = 0;
    mb->normal_count = 0;

    mb->vertices = malloc( sizeof(f32) * mb->vertex_size );
    mb->indices  = malloc( sizeof(u32) * mb->index_size  );
    mb->uvs      = malloc( sizeof(f32) * mb->uv_size     );
    mb->normals  = malloc( sizeof(f32) * mb->normal_size );
    
    return mb;
}

static void mesh_ensure_capacity(Mesh_builder* mb) {

    if (mb->vertex_count + 12 == mb->vertex_size) {
        mb->vertex_size += MAX_VERTEX_FLOATS / 10;
        mb->vertices = realloc( mb->vertices, mb->vertex_size * sizeof(f32) );
    }

    if (mb->index_count + 6 == mb->index_size) {
        mb->index_size += MAX_INDICES / 10;
        mb->indices = realloc( mb->indices, mb->index_size * sizeof(u32) );
    }

    if (mb->uv_count + 8 == mb->uv_size) {
        mb->uv_size += MAX_UVS / 10;
        mb->uvs = realloc( mb->uvs, mb->uv_size * sizeof(f32) );
    }

    if (mb->normal_count + 12 == mb->normal_size) {
        mb->normal_size += MAX_NORMALS / 10;
        mb->normals = realloc( mb->normals, mb->normal_size * sizeof(f32) );
    }

}

static void mesh_clamp(Mesh_builder* mb) {

    mb->vertices = realloc( mb->vertices, mb->vertex_count * sizeof(f32) );
    mb->indices  = realloc(  mb->indices,  mb->index_count * sizeof(u32) );
    mb->uvs      = realloc(      mb->uvs,     mb->uv_count * sizeof(f32) );
    mb->normals  = realloc(  mb->normals, mb->normal_count * sizeof(f32) );

}

static void free_mesh_builder(Mesh_builder* mb) {  
    free( mb->vertices );
    free( mb->indices  );
    free( mb->uvs      );
    free( mb->normals  );
    free( mb );
}

static const f32 CUBE_VERTICES[72] = {

    // looking at north face
    0, 0, 0,    
    0, 1, 0,    
    1, 1, 0,    
    1, 0, 0,    

    // south face
    1, 0, 1,    
    1, 1, 1,    
    0, 1, 1,    
    0, 0, 1,    

    // west face
    0, 0, 1,    
    0, 1, 1,   
    0, 1, 0,    
    0, 0, 0,    

    // east face
    1, 0, 0,    
    1, 1, 0,  
    1, 1, 1,   
    1, 0, 1,    

    // bottom face
    0, 0, 1, 
    0, 0, 0,    
    1, 0, 0,   
    1, 0, 1,    

    // top face
    0, 1, 0,   
    0, 1, 1,    
    1, 1, 1,    
    1, 1, 0,    
};

static const f32 CUBE_NORMALS[18] = {

     0.0f,  0.0f, -1.0f,   // Z-
     0.0f,  0.0f, +1.0f,   // Z+
    -1.0f,  0.0f,  0.0f,   // X-
    +1.0f,  0.0f,  0.0f,   // X+
     0.0f, -1.0f,  0.0f,   // Y-
     0.0f, +1.0f,  0.0f,   // Y+
};

void init_chunk(Chunk *chunk, u16 pos_x, u16 pos_z, fnl_state* noise_state) {
     chunk->dirty = true;
     chunk->x = pos_x;
     chunk->z = pos_z;

    for (int x = 0; x < CHUNK_LENGTH; x++) {
        for (int z = 0; z < CHUNK_LENGTH; z++) {

        f32 w_x = chunk->x * CHUNK_LENGTH + x;
        f32 w_z = chunk->z * CHUNK_LENGTH + z;
        f32 noise2d = fnlGetNoise2D(noise_state, w_x, w_z);
        noise2d = ( noise2d + 1.0f ) / 2.0f;
        u16 height = (u16)(noise2d  * (CHUNK_HEIGHT - 1));
        

            for (int y = 0; y < CHUNK_HEIGHT; y++) {
            

                if (y < height / 2)
                    chunk->blocks[x][y][z] = STONE;
                else if (y < height) 
                    chunk->blocks[x][y][z] = DIRT;
                else if (y == height) 
                    chunk->blocks[x][y][z] = GRASS;
                else 
                    chunk->blocks[x][y][z] = AIR;


            }
        }
    }
}

static void add_face(Mesh_builder *mb, Chunk *chunk, block_face face, u16 x, u16 y, u16 z) {

    mesh_ensure_capacity(mb);

    BlockID block = chunk->blocks[x][y][z];

    u8 cube_offset = face * 4 * 3;
    u32 vertex_offset = mb->vertex_count / 3;

    static const u8 index_template[6] = { 0, 1, 2,      2, 3, 0, };

    s16 world_x = chunk->x * CHUNK_LENGTH;
    s16 world_z = chunk->z * CHUNK_LENGTH;
                    
    for (int i = 0; i < 12; i += 3) {
        mb->vertices[ (mb->vertex_count)++ ] = CUBE_VERTICES[cube_offset + i + 0] + x + world_x;
        mb->vertices[ (mb->vertex_count)++ ] = CUBE_VERTICES[cube_offset + i + 1] + y;
        mb->vertices[ (mb->vertex_count)++ ] = CUBE_VERTICES[cube_offset + i + 2] + z + world_z;
    }

    for (int i = 0; i < 6; i++) {
        mb->indices[ (mb->index_count)++ ] = index_template[i] + vertex_offset;
    }

    vec2 min;
    glm_vec2_copy(Block_Register[block].texmaps[face].min, min);
    vec2 max;
    glm_vec2_copy(Block_Register[block].texmaps[face].max, max);

    mb->uvs[ (mb->uv_count)++ ] = min[0];
    mb->uvs[ (mb->uv_count)++ ] = min[1];

    mb->uvs[ (mb->uv_count)++ ] = min[0];
    mb->uvs[ (mb->uv_count)++ ] = max[1];

    mb->uvs[ (mb->uv_count)++ ] = max[0];
    mb->uvs[ (mb->uv_count)++ ] = max[1];

    mb->uvs[ (mb->uv_count)++ ] = max[0];
    mb->uvs[ (mb->uv_count)++ ] = min[1];

    for (int i = 0; i < 4; i++)
        for (int k = 0; k < 3; k++)
            mb->normals[ (mb->normal_count)++ ] = CUBE_NORMALS[ k + (face * 3) ];

}

static void make_buffer_objects(Chunk *chunk, Mesh_builder* mb) {

    mesh_clamp(mb);

    glGenVertexArrays(1, &chunk->VAO);
    glBindVertexArray(chunk->VAO);

    glGenBuffers(1, &chunk->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->VBO);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(f32) * mb->vertex_count,
        mb->vertices, GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glGenBuffers(1, &chunk->UVBO);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->UVBO);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(f32) * mb->uv_count, 
        mb->uvs, GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glGenBuffers(1, &chunk->NVBO);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->NVBO);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(f32) * (mb->normal_count), 
        mb->normals, GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glGenBuffers(1, &chunk->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * mb->index_count,
        mb->indices, GL_STATIC_DRAW
    );

    glBindVertexArray(0);

    chunk->index_count = mb->index_count;
}

static Chunk *dummy_chunk() {
    static Chunk dummy;
    static bool did_init = false;
    if (!did_init) {
        memset(dummy.blocks, AIR, sizeof(dummy.blocks));
        did_init = true;
    }

    return &dummy;
}

// change from taking in a chunk and the world
// to just taking in the world, then x and z coordinates
void clean_chunk(Chunk *chunk, World *world) {
    if (!chunk->dirty) return;

    Mesh_builder *mesh_builder = new_mesh_builder();

   // world-array chunk x and z coords
    u16 chunk_WAX = chunk->x + (WORLD_CHUNK_LIMIT / 2),
        chunk_WAZ = chunk->z + (WORLD_CHUNK_LIMIT / 2),
        max = WORLD_CHUNK_LIMIT;

    Chunk *dummy = dummy_chunk();

    Chunk *x_pos = (chunk_WAX + 1 < max) ? &world->chunks[chunk_WAX + 1][chunk_WAZ] : dummy, 
          *x_neg = (chunk_WAX > 0)       ? &world->chunks[chunk_WAX - 1][chunk_WAZ] : dummy, 
          *z_pos = (chunk_WAZ + 1 < max) ? &world->chunks[chunk_WAX][chunk_WAZ + 1] : dummy, 
          *z_neg = (chunk_WAZ > 0)       ? &world->chunks[chunk_WAX][chunk_WAZ - 1] : dummy;


    // when done testing single chunk add checks for adjacent chunks

    for (int x = 0; x < CHUNK_LENGTH; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_LENGTH; z++) {
                if (chunk->blocks[x][y][z] == AIR) continue;

                //east face check
                if ( x + 1 == CHUNK_LENGTH ) {
                    if (Block_Register[ x_pos->blocks[0][y][z] ].opacity < 15)
                        add_face( mesh_builder, chunk, face_east, x, y, z );
                }
                else {
                    if (Block_Register[ chunk->blocks[x + 1][y][z] ].opacity < 15)
                        add_face( mesh_builder, chunk, face_east, x, y, z );
                }

                //west face check
                if ( x - 1 < 0 ) {
                    if (Block_Register[ x_neg->blocks[CHUNK_LENGTH - 1][y][z] ].opacity < 15)
                        add_face( mesh_builder, chunk, face_west, x, y, z );
                }
                else {
                    if (Block_Register[ chunk->blocks[x - 1][y][z] ].opacity < 15)
                        add_face( mesh_builder, chunk, face_west, x, y, z );
                }

                //top face check
                if ( y + 1 >= CHUNK_HEIGHT - 1 || 
                    Block_Register[ chunk->blocks[x][y + 1][z] ].opacity < 15 ) {
                        add_face( mesh_builder, chunk, face_up, x, y, z );
                }
                //bottom face check
                if ( y == 0 || 
                    Block_Register[ chunk->blocks[x][y - 1][z] ].opacity < 15 ) {
                        add_face( mesh_builder, chunk, face_down, x, y, z );
                }


                //south face check
                if ( z + 1 == CHUNK_LENGTH ) {
                    if (Block_Register[ z_pos->blocks[x][y][0] ].opacity < 15)
                        add_face( mesh_builder, chunk, face_south, x, y, z );
                }
                else {
                    if (Block_Register[ chunk->blocks[x][y][z + 1] ].opacity < 15)
                        add_face( mesh_builder, chunk, face_south, x, y, z );
                }


                //north face check
                if ( z - 1 < 0 ) {
                    if (Block_Register[ z_neg->blocks[x][y][CHUNK_LENGTH - 1] ].opacity < 15)
                        add_face( mesh_builder, chunk, face_north, x, y, z );
                }
                else {
                    if (Block_Register[ chunk->blocks[x][y][z - 1] ].opacity < 15)
                        add_face( mesh_builder, chunk, face_north, x, y, z );
                }

            }
        }
    }


    make_buffer_objects( chunk, mesh_builder );

    chunk->dirty = false;

    free_mesh_builder( mesh_builder );
}

void render_chunk(Chunk *chunk) {
    glBindVertexArray(chunk->VAO);
    glDrawElements(
        GL_TRIANGLES, chunk->index_count, GL_UNSIGNED_INT, (void*)0
    );
}

void free_chunk(Chunk *chunk) {
    glDeleteVertexArrays(1, &chunk->VAO);
    glDeleteBuffers(1, &chunk->VBO);
    glDeleteBuffers(1, &chunk->EBO);
    glDeleteBuffers(1, &chunk->UVBO);
    glDeleteBuffers(1, &chunk->NVBO);
    free(chunk);
}