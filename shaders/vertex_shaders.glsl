#version 410 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 texture_id;
uniform mat4 MVP;


out vec2 texture_atlas_id;

void main() {
    gl_Position = MVP * vec4(a_pos, 1.0);
    texture_atlas_id = texture_id;
}
