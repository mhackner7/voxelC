#include "input.h"

#include "gfx_libs.h"

#include <cglm/cglm.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>
#include <cglm/mat4.h>
#include <math.h>

void handle_key_pst(Camera *camera, f64 dt) {

    SDL_PumpEvents();
    const u8 *keys = SDL_GetKeyboardState(NULL);

    f32 speed = ( keys[SDL_SCANCODE_LSHIFT] ) ? camera->speed * 5 : camera->speed;
    

    vec3 movement = GLM_VEC3_ZERO_INIT;
    vec3 temp     = GLM_VEC3_ZERO_INIT;

    glm_vec3_cross(camera->front, camera->world_up, camera->right);
    glm_vec3_normalize(camera->right);

    if ( keys[SDL_SCANCODE_W] ) {
        f32 y_capture = movement[1];
        glm_vec3_add(movement, camera->front, movement);
        movement[1] = y_capture;
        // printf("W pressed");
    }

    if ( keys[SDL_SCANCODE_S] ) {
        f32 y_capture = movement[1];
        glm_vec3_negate_to(camera->front, temp);
        glm_vec3_add(movement, temp, movement);
        movement[1] = y_capture;
        // printf("S pressed");
    }

    if ( keys[SDL_SCANCODE_A] ) {
        glm_vec3_negate_to(camera->right, temp);
        glm_vec3_add(movement, temp, movement);
        // printf("A pressed");
    }

    if ( keys[SDL_SCANCODE_D] ) {
        glm_vec3_add(movement, camera->right, movement);
        // printf("D pressed");
    }

    
    glm_vec3_normalize(movement);
    glm_vec3_scale(movement, (speed * dt), movement);
    glm_vec3_add(camera->position, movement, camera->position);

    if ( keys[SDL_SCANCODE_SPACE] ) {
        camera->position[1] += speed * dt;
        // printf("Space pressed");
    }

    if ( keys[SDL_SCANCODE_C] || keys[SDL_SCANCODE_RSHIFT] ) {
        camera->position[1] -= speed * dt;
        // printf("Shift pressed");
    }

}


void handle_mouse_pst
(SDL_Event *event, Camera *camera, Window *window, f64 dt) {

    s32 rel_x = event->motion.xrel * camera->sensitivity;
    s32 rel_y = event->motion.yrel * camera->sensitivity;

    //printf("rel x: %d, rel y: %d\n", rel_x, rel_y);
    
    camera->pitch -= rel_y * dt;
    if (camera->pitch >  89.0f) camera->pitch  =  89.0f;
    if (camera->pitch < -89.0f) camera->pitch  = -89.0f;
    camera->yaw += rel_x * dt;


    SDL_WarpMouseInWindow(window->sdl_handle, window->width / 2, window->height / 2);
    SDL_ShowCursor(SDL_DISABLE);
}


//return true only if q pressed
bool handle_keytaps (SDL_Event *event, f64 dt) {

    static bool toggle_wireframe = false;

    switch (event->key.keysym.sym) {
        case SDLK_t:

            if (!toggle_wireframe) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    toggle_wireframe = true;
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                toggle_wireframe = false;
            }
            break;

        case SDLK_q:
            return true;
            break;
        

        default:
            break;
        


    }

    return false;
}