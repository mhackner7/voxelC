#include "texture.h"

#include "gfx_libs.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

Atlas load_atlas(const char *filename) {

    fprintf( stderr, "Loading atlas: %s\n", filename );

    Atlas atlas;

    stbi_set_flip_vertically_on_load(1);

    stbi_uc *stb_handle = stbi_load(
        filename, &atlas.x, &atlas.y, &atlas.channels, STBI_rgb_alpha
    );

    if (!stb_handle) {
        fprintf( stderr, "Error: texture atlas failed to load %s\n", stbi_failure_reason() );
        exit(1);
    }

    atlas.tile_size = 16;

    glGenTextures(1, &atlas.handle);
    glBindTexture(GL_TEXTURE_2D, atlas.handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLenum channels = (atlas.channels == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, atlas.x, atlas.y, 
        0, channels, GL_UNSIGNED_BYTE, stb_handle
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(stb_handle);

    fprintf( stderr, "Atlas Loaded\n" );

    return atlas;
}

UVs tex_get_uvs(u16 x, u16 y) {
    // size in pixels
    static const u8  tile_size  = 16;
    static const u16 atlas_size = 256;

    return (UVs) {
        .min[0] = (f32)((x * tile_size) - tile_size) / (f32)atlas_size,
        .min[1] = (f32)((y * tile_size) - tile_size) / (f32)atlas_size,

        .max[0] = (f32)(x * tile_size) / (f32)atlas_size,
        .max[1] = (f32)(y * tile_size) / (f32)atlas_size,
    };
}