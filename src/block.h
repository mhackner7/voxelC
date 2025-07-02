#pragma once

#include "types.h"
#include "texture.h"


typedef enum {
    AIR,          
    DIRT,       
    GRASS,        
    STONE,
    SAND,
    OAK_WOOD,
    OAK_LEAVES,
    ROSE,
    WATER,
    LAVA,
    BLOCK_TOTAL_COUNT,
} BlockID;


typedef enum {
    TYPE_AIR,
    TYPE_SOLID,
    TYPE_LIQUID,
    TYPE_PLANT,
    TYPE_ATTATCHED,
    TYPE_MISC
} BlockType;


typedef enum {
    face_north,
    face_south,
    face_west,
    face_east,
    face_down,
    face_up,
} block_face;

typedef struct {
    UVs                 texmaps[6];
    //  ^^^ float tuple that stores the (u,v)'s indexed by face (see above enum)
    u8                  opacity;                 
    //  ^^^ 0 - 15, 0 for air ; 15 for solid
    BlockType           type;
    bool                is_solid;
    bool                has_gravity;
    bool                is_flammable;
    bool                can_light;
    bool                can_power;
    bool                can_interact;
    /*
    - sound & particle data
    */
} GlobalBlock;

GlobalBlock Block_Register[BLOCK_TOTAL_COUNT];

void init_blockregister();

typedef u8 Block;
typedef u8 metadata;



/*     16 bit uint to act as register id (8 bits) and metadata (8 bits)
                                         bits 0-7             bits 8-15



//         



active status - 1 bit
i.e. water flowing / light on / redstone powered / furnace cooking
bit 8
*/
#define _active_flag        (  (u32)1 << 8         )
#define set_active(block)   (  (block) |= _active_flag   )
#define set_inactive(block) (  (block) &= ~_active_flag  )
#define is_active(block)    (  ((block & _active_flag) == _active_flag) ? 1 : 0  )


/* 
direction - 3 bits - facing north / south / east / west / up / down
bits 9, 10, 11
001 - north
010 - east
011 - south
100 - west
101 - up
110 - down
111 - clear
-- which direction the primary face is facing
-- rotates textures accordingly
*/
typedef enum {
    DIR_NORTH   = 1,
    DIR_SOUTH   = 2,
    DIR_EAST    = 3,
    DIR_WEST    = 4,
    DIR_DOWN    = 5,
    DIR_UP      = 6,
} block_face_direction;

#define DIR_SHIFT 9
#define DIR_NORTH    (u16)( DIR_NORTH << DIR_SHIFT )
#define DIR_EAST     (u16)( DIR_EAST  << DIR_SHIFT )
#define DIR_SOUTH    (u16)( DIR_SOUTH << DIR_SHIFT )
#define DIR_WEST     (u16)( DIR_WEST  << DIR_SHIFT )
#define DIR_UP       (u16)( DIR_UP    << DIR_SHIFT )
#define DIR_DOWN     (u16)( DIR_DOWN  << DIR_SHIFT )

#define _clear_dir(block) (  (block) &= ~( (u16)(0b111 << DIR_SHIFT) )  )

void block_set_direction(u16* block, u8 direction);

#define set_dir_north(block)    \
    do {                        \
        _clear_dir(block);      \
        (block) |= _NORTH;      \
    } while (0)

#define set_dir_east(block)     \
    do {                        \
        _clear_dir(block);      \
        (block) |= _EAST;       \
    } while (0)

#define set_dir_south(block)    \
    do {                        \
        _clear_dir(block);      \
        (block) |= _SOUTH;      \
    } while (0)

#define set_dir_west(block)     \
    do {                        \
        _clear_dir(block);      \
        (block) |= _WEST;       \
    } while (0)

#define set_dir_up(block)       \
    do {                        \
        _clear_dir(block);      \
        (block) |= _UP;         \
    } while (0)

#define set_dir_down(block)     \
    do {                        \
        _clear_dir(block);      \
        (block) |= _DOWN;       \
    } while (0)

#define get_direction(block) ((block_face_direction)(   (block & _CLEAR) >> DIR_SHIFT   ))

/* 
lighting - 4 bits - 16 possible light levels
bits 12, 13, 14, 15
*/
#define LIGHT_SHIFT 12
#define LIGHT_MASK ((uint32_t)(0xF << LIGHT_SHIFT))

#define set_light_level(block, level)       \
    do {                                    \
        (block) &= ~LIGHT_MASK;             \
        (block) |= (level) << LIGHT_SHIFT;  \
    } while (0)

#define get_light_level(block) ( ( (block) & LIGHT_MASK ) >> LIGHT_SHIFT)

