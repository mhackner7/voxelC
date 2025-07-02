#include "camera.h"

void init_cam(Camera *camera) {
    vec3 pos = {0.5f, 150.0f, 32.0f};
    glm_vec3_copy(pos, camera->position);

    vec3 world_up = {0.0f, 1.0f, 0.0f};
    glm_vec3_copy(world_up, camera->world_up);

    camera->yaw   = -90.0f;
    camera->pitch =  0.0f;
    camera->fov   =  100.0f;
    camera->speed =  20.0f;
    camera->sensitivity = 50.0f;
}

void update_camera_vectors(Camera *camera) {
    
    f32 yaw_radians   =  glm_rad(camera->yaw);
    f32 pitch_radians =  glm_rad(camera->pitch);

    vec3 direction;
    direction[0]  =  cosf(yaw_radians) * cosf(pitch_radians);
    direction[1]  =  sinf(pitch_radians);
    direction[2]  =  sinf(yaw_radians) * cosf(pitch_radians);
    glm_vec3_normalize(direction);

    glm_vec3_copy(direction, camera->front);

    glm_vec3_cross(camera->front, camera->world_up, camera->right);
    glm_vec3_normalize(camera->right);
    
}

void update_mvp(Camera *camera, Window *window, GLuint shader_program) {

    vec3 center;
    glm_vec3_add(camera->position, camera->front, center);

    mat4 model = GLM_MAT4_IDENTITY_INIT, view, projection;

    glm_lookat(camera->position, center, camera->world_up, view);

    glm_perspective(
        glm_rad(camera->fov), 
        get_aspect_ratio(window), 
        .1f, 1000.0f, 
        projection
    );

    mat4 mvp;

    glm_mat4_mul(projection, view, mvp);
    glm_mat4_mul(mvp, model, mvp);

    glUseProgram(shader_program);

    glUniformMatrix4fv( 
        glGetUniformLocation(shader_program, "MVP"), 
        1, GL_FALSE, &mvp[0][0] 
    );

}


