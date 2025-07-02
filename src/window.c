#include "window.h"


void init_window(Window *window) {
    window->width  = 1280;
    window->height = 720;
    window->show_wireframe = false;

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    window->sdl_handle = SDL_CreateWindow("window", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,   
        window->width, window->height, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (window->sdl_handle == NULL) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }
    

    SDL_GLContext gl_context = SDL_GL_CreateContext(window->sdl_handle);

    if (!gl_context) {
        fprintf(stderr, "GL context could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }

    SDL_GL_MakeCurrent(window->sdl_handle, gl_context);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        fprintf(stderr,"Failed to initialize GLAD\n");
        exit(1);
    }    

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LESS);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);  

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void end_window(Window *window) {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

f32 get_aspect_ratio(Window *window) {
    return (f32)(window->width) / window->height;
}