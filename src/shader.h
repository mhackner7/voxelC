#pragma once

#include "gfx_libs.h"
#include <stdlib.h>
#include "types.h"

char* read_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "error opening file: %s\n", filename);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char *code = (char*)malloc(size + 1);
    if (!code) { 
        fprintf(stderr, "error allocating shader-string buffer");
        fclose(file); 
        exit(1);
    }

    if (fread(code, 1, size, file) != size) {
        fprintf(stderr, "error reading file: %s into string buffer\n", filename);
        free(code);
        fclose(file);
        exit(1);
    }
    code[size] = '\0';
    fclose(file);
    return code;
}

GLuint LoadShaders() {
    GLuint vertex_id = glCreateShader(GL_VERTEX_SHADER);
    char* vertex_source = read_file("../shaders/vertex_shaders.glsl");
    glShaderSource(vertex_id, 1, &vertex_source, NULL);
    glCompileShader(vertex_id);

    GLuint fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    char* fragment_source = read_file("../shaders/fragment_shaders.glsl");
    glShaderSource(fragment_id, 1, &fragment_source, NULL);
    glCompileShader(fragment_id);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_id);
    glAttachShader(shader_program, fragment_id);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

    return shader_program;
}