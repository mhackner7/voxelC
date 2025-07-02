#pragma once

#include "types.h"
#include "gfx_libs.h"
#include "window.h"


typedef struct Camera {
    vec3    world_up;
    vec3    position;
    vec3    front;
    vec3    right;
    f32     yaw;
    f32     pitch;
    f32     speed;
    f32     sensitivity;
    f32     fov;
} Camera;

void init_cam(Camera *camera);

void update_camera_vectors(Camera *camera);

void update_mvp(Camera *camera, Window *window, GLuint shader_program);
