#include "world.h"
#include <stdio.h>

#include <stdlib.h>
#include <errno.h>

#include <stdio.h>
#include <stdbool.h>

#ifdef _WIN32
  #include <windows.h>
  #include <direct.h>   // _mkdir, _chdir
  #define PATH_SEPARATOR '\\'
#else
  #include <sys/stat.h> // mkdir, stat
  #include <unistd.h>   // chdir
  #define PATH_SEPARATOR '/'
#endif

static bool dir_exists(const char *directory) {
    #ifdef _WIN32
        DWORD attr = GetFileAttributesA(directory);
        return (attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY);
    #else
        struct stat info;
        if (stat(directory, &info) != 0) return false;
        return (info.st_mode & S_IFDIR) != 0;
    #endif
}

static bool change_dir(const char *directory) {
    #ifdef _WIN32
        return _chdir(directory) == 0;
    #else
        return chdir(directory) == 0;
    #endif
}

static bool create_dir(const char *directory) {
    if ( dir_exists(directory) ) return true; // already exists

    #ifdef _WIN32
        return _mkdir(directory) == 0;
    #else
        // 0755 = rwxr-xr-x permissions
        return mkdir(directory, 0755) == 0;
    #endif
}

static void save_chunk_row(Chunk **chunk_row, u8 array_x);
static void load_chunk_row(Chunk **chunk_row, u8 array_x);
static void load_world(World *world);
static bool dirname_is_valid(const char *dirname);



World *create_world() {

    init_blockregister();


    fprintf( stderr, "Allocating World\n" );
    World *world = malloc(sizeof(*world));

    world->state = fnlCreateState(time(NULL));
    world->state.noise_type = FNL_NOISE_PERLIN;
    world->state.frequency = 0.003f;
    world->state.fractal_type = FNL_FRACTAL_FBM;
    world->state.octaves = 4;
    world->state.lacunarity = 2.173f;
    world->state.gain = 0.66f;



    s16 offset = (WORLD_CHUNK_LIMIT / 2);


    fprintf( stderr, "Building World\n" );
    for (int i = 0; i < WORLD_CHUNK_LIMIT; i++) {
        for (int k = 0; k < WORLD_CHUNK_LIMIT; k++) {
            init_chunk(
                &world->chunks[i][k], 
                (i - offset), 
                (k - offset),
                &world->state
            );
        }
    }

    fprintf( stderr, "Making World Mesh\n" );

    for (int i = 0; i < WORLD_CHUNK_LIMIT; i++) {

        fprintf( 
                stderr, 
                "Meshing chunk row%d\n", 
                i
            );
        for (int k = 0; k < WORLD_CHUNK_LIMIT; k++) {
            
            clean_chunk(&world->chunks[i][k], world);
        }
    }

    fprintf( stderr, "World created!\n" );

    return world;
}

void render_world(World* world) {
    for (int i = 0; i < WORLD_CHUNK_LIMIT; i++) {
        for (int k = 0; k < WORLD_CHUNK_LIMIT; k++) {
            render_chunk(&world->chunks[i][k]);
        }
    }
}


/*
this must be changed when multiple worlds/saves are implemented

each world will be a seperate compressed folder of 64 row files 
                                        (each storing a row of chunks)

world/filename



*/
World *init_world(char *save_name, bool is_new, u32 seed) {

    World *world = malloc(sizeof(*world));
    

    if ( !dirname_is_valid(save_name) ) {
        fprintf(stderr, "FATAL ERROR: invalid directory name %s", save_name);
        exit(1);
    }
    else strncpy(world->save_name, save_name, SAVE_NAME_LIMIT - 1);

    if (is_new) {

        //generate noise for world from seed 

        for (int i = 0; i < WORLD_CHUNK_LIMIT; i++) {
            for (int k = 0; k < WORLD_CHUNK_LIMIT; k++) {
                //make the new world's chunks

            }
        }
    }

    else {
        load_world(&world);

        for (int i = 0; i < WORLD_CHUNK_LIMIT; i++) {
            for (int k = 0; k < WORLD_CHUNK_LIMIT; k++) {
                clean_chunk(&world->chunks[i][k], world);
            }
        }
    }

    return world;
}

void save_world(World *world) {

    if ( !dir_exists(world->save_name) ) {
        if ( !create_dir(world->save_name) ) { 
            fprintf(stderr, "FATAL ERROR: creating directory %s", world->save_name);
            exit(1);
        }
        if ( !change_dir(world->save_name) ) {
            fprintf(stderr, "FATAL ERROR: changing to directory %s", world->save_name);
            exit(1);
        }
    }

    for (int i = 0; i < WORLD_CHUNK_LIMIT; i++)
        save_chunk_row(world->chunks[i], i);

}

static void load_world(World *world) {
    
    if ( !dir_exists(world->save_name) ) {
        fprintf(stderr, "FATAL ERROR: cannot load directory %s", world->save_name);
        exit(1);
    }

    if ( !change_dir(world->save_name) ) {
        fprintf(stderr, "FATAL Error changing to directory %s", world->save_name);
        exit(1);
    }

    for (int i = 0; i < WORLD_CHUNK_LIMIT; i++)
        load_chunk_row(world->chunks[i], i);

}

static void save_chunk_row(Chunk **chunk_row, u8 array_x) {

    char filename[14] = "_row_XX_.chunk\0";
    filename[5] = '0' + ( array_x / 10 );
    filename[6] = '0' + ( array_x % 10 );

    fprintf(stderr, "saving row [%d] at %s\n", array_x, filename);

    FILE *file = fopen(filename, "wb");

    if ( !file) {
        fprintf(stderr, "FATAL ERROR: saving to file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < WORLD_CHUNK_LIMIT; i++) {


        fwrite( &chunk_row[i]->x,      sizeof(chunk_row[i]->x),      1,  file );
        fwrite( &chunk_row[i]->z,      sizeof(chunk_row[i]->z),      1,  file );
        fwrite( chunk_row[i]->blocks,  sizeof(chunk_row[i]->blocks), 1,  file );
        fwrite( chunk_row[i]->meta,    sizeof(chunk_row[i]->meta),   1,  file );

    }

    fclose(file);
}

static void load_chunk_row(Chunk **chunk_row, u8 array_x) {

    char filename[14] = "_row_XX_.chunk\0";
    filename[5] = '0' + ( array_x / 10 );
    filename[6] = '0' + ( array_x % 10 );

    fprintf(stderr, "loading row [%d] at %s\n", array_x, filename);

    FILE *file = fopen(filename, "rb");

    if ( !file) {
        fprintf(stderr, "FATAL ERROR: loading file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < WORLD_CHUNK_LIMIT; i++) {

        fread( &chunk_row[i]->x,      sizeof(chunk_row[i]->x),      1,  file );
        fread( &chunk_row[i]->z,      sizeof(chunk_row[i]->z),      1,  file );
        fread( chunk_row[i]->blocks,  sizeof(chunk_row[i]->blocks), 1,  file );
        fread( chunk_row[i]->meta,    sizeof(chunk_row[i]->meta),   1,  file );

    }

    fclose(file);
}

static bool dirname_is_valid(const char *dirname) {
    if (!dirname) return false;
    return strlen(dirname) < SAVE_NAME_LIMIT;
}


