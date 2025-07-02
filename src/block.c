#include "block.h"

void block_set_direction(u16 *block, u8 direction) {
    if (direction < 0 && direction > 6) {
        printf("error setting block direction, invalid direction");
        exit(1);
    }

    *block &= ~( (u16)(0b111 << DIR_SHIFT) );

    if (direction == 0) *block |= DIR_NORTH;
    else *block |= direction;
}

/* 
    change:
        - textureid
        - xyz
        - durability
        - health
    add: 
        - sound 

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

    .texmaps[6] = {},
    .opacity = ,                
    .type = ,
    .is_solid = ,
    .has_gravity = ,
    .is_flammable = ,
    .can_light = ,
    .can_power = ,
    .can_interact = ,

*/


static void init_AIR() {
    GlobalBlock air = {
        .texmaps = {}, 
        .opacity = 0,                
        .type = TYPE_AIR,
        .is_solid = false,
        .has_gravity = false,
        .is_flammable = false,
        .can_light = false,
        .can_power = false,
        .can_interact = false,
    };

    Block_Register[AIR] = air;
}

static void init_DIRT() {
    UVs uv = tex_get_uvs(1, 1);
    GlobalBlock dirt = {
        .texmaps = {
            [face_north] = uv,
            [face_south] = uv,
            [face_east]  = uv,
            [face_west]  = uv,
            [face_down]  = uv,
            [face_up]    = uv,
        }, 
        .opacity = 15,                
        .type = TYPE_SOLID,
        .is_solid = true,
        .has_gravity = false,
        .is_flammable = false,
        .can_light = false,
        .can_power = false,
        .can_interact = true,
    };

    Block_Register[DIRT] = dirt;
}

static void init_GRASS() {
    UVs uv = tex_get_uvs(1, 2);
    GlobalBlock grass = {
        .texmaps = {
            [face_north] = uv,
            [face_south] = uv,
            [face_west]  = uv,
            [face_east]  = uv,
            [face_down]  = tex_get_uvs(1, 1),
            [face_up]    = tex_get_uvs(1, 3),
        }, 
        .opacity = 15,                
        .type = TYPE_SOLID,
        .is_solid = true,
        .has_gravity = false,
        .is_flammable = false,
        .can_light = false,
        .can_power = false,
        .can_interact = true,
    };

    Block_Register[GRASS] = grass;
}

static void init_STONE() {
    UVs uv = tex_get_uvs(6, 1);
    GlobalBlock stone = {
        .texmaps = {
            [face_north] = uv,
            [face_south] = uv,
            [face_east]  = uv,
            [face_west]  = uv,
            [face_down]  = uv,
            [face_up]    = uv,
        }, 
        .opacity = 15,                
        .type = TYPE_SOLID,
        .is_solid = true,
        .has_gravity = false,
        .is_flammable = false,
        .can_light = false,
        .can_power = false,
        .can_interact = true,
    };

    Block_Register[STONE] = stone;
}

static void init_SAND() {
    UVs uv = tex_get_uvs(2, 1);
    GlobalBlock sand = {
        .texmaps = {
            [face_north] = uv,
            [face_south] = uv,
            [face_east]  = uv,
            [face_west]  = uv,
            [face_down]  = uv,
            [face_up]    = uv,
        }, 
        .opacity = 15,                
        .type = TYPE_SOLID,
        .is_solid = true,
        .has_gravity = true,
        .is_flammable = false,
        .can_light = false,
        .can_power = false,
        .can_interact = true,
    };

    Block_Register[SAND] = sand;
}

static void init_OAK_WOOD() {
    UVs uv = tex_get_uvs(5, 2);
    GlobalBlock oak_wood = {
        .texmaps = {
            [face_north] = uv,
            [face_south] = uv,
            [face_east]  = uv,
            [face_west]  = uv,
            [face_down]  = tex_get_uvs(5, 1),
            [face_up]    = tex_get_uvs(5, 3),
        }, 
        .opacity = 15,                
        .type = TYPE_SOLID,
        .is_solid = true,
        .has_gravity = false,
        .is_flammable = true,
        .can_light = false,
        .can_power = false,
        .can_interact = true,
    };

    Block_Register[OAK_WOOD] = oak_wood;
}

static void init_OAK_LEAVES() {
    UVs uv = tex_get_uvs(15, 1);
    GlobalBlock oak_leaves = {
        .texmaps = {
            [face_north] = uv,
            [face_south] = uv,
            [face_east]  = uv,
            [face_west]  = uv,
            [face_down]  = uv,
            [face_up]    = uv,
        }, 
        .opacity = 0,                
        .type = TYPE_SOLID,
        .is_solid = true,
        .has_gravity = false,
        .is_flammable = true,
        .can_light = false,
        .can_power = false,
        .can_interact = true,
    };

    Block_Register[OAK_LEAVES] = oak_leaves;
}

static void init_ROSE() {}

static void init_WATER() {}

static void init_LAVA() {}

void init_blockregister() {
    init_AIR();
    init_DIRT();
    init_GRASS();
    init_STONE();
    init_SAND();
    init_OAK_WOOD();
    init_OAK_LEAVES();
    // init_ROSE();
    // init_WATER();
    // init_LAVA();
}