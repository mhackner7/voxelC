#include "types.h"
#include "window.h"
#include "shader.h"
#include "camera.h"
#include "input.h"
#include "world.h"


#include <stdio.h>


int main() {

    Window window;
    init_window(&window);

    SDL_SetRelativeMouseMode(true);

    GLuint shader_program = LoadShaders();

    Atlas texture_atlas = load_atlas("../assets/textures/block_atlas.png");

    Camera camera;
    init_cam(&camera);
    update_camera_vectors(&camera);
    update_mvp(&camera, &window, shader_program);

    World *world = create_world();
    
    SDL_GL_SwapWindow(window.sdl_handle);

    u64 now_time   =  SDL_GetPerformanceCounter();
    u64 last_time  =  0;
    f64 delta_time =  0;

    bool quit = false;
    SDL_Event event;
    while (!quit) {

        last_time = now_time;
        now_time = SDL_GetPerformanceCounter();
        delta_time = (now_time - last_time) / (f64)(SDL_GetPerformanceFrequency());

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                quit = handle_keytaps(&event, delta_time);
            }

            else if (event.type == SDL_MOUSEMOTION) {

                handle_mouse_pst(&event, &camera, &window, delta_time);
            }
            else if (event.type == SDL_QUIT) quit = true;
        
        }
        update_camera_vectors(&camera);
        handle_key_pst(&camera, delta_time);
        update_mvp(&camera, &window, shader_program);

        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_program);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_atlas.handle);
        glUniform1i( glGetUniformLocation(shader_program, "my_texture"), 0);


        render_world(world);


        SDL_GL_SwapWindow(window.sdl_handle);

    }

    glDeleteProgram(shader_program);
    
    end_window(&window);
    return 0;
}

