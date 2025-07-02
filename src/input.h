#pragma once

#include "types.h"
#include "camera.h"
#include "window.h"


/**
 * persistent:
 * pst function -> expecting key to be held or rapid changes
 * very fast polling (every frame)
 * 
 * persistent keys include:
 * movement - W/A/S/D UP/DOWN Crouching, sprinting, mining
 * mouse movement - constantly changing mouse location
 * 
 * tap:
 * tp function -> expecting keys to be tapped
 * slow polling (every 100-200ms?)
 * 
 * tap keys include:
 * toggles - show wireframes
 * taps - show menu/inventory
 * interacting - placing, attacking
 * 
 */


//true if the game continues, false if a quit is read
//will change later
void handle_key_pst(Camera *camera, f64 dt);

void handle_mouse_pst
(SDL_Event *event, Camera *camera, Window *window, f64 dt);

bool handle_keytaps (SDL_Event *event, f64 dt);


