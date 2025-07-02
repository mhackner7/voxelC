#version 410 core

in vec2 texture_atlas_id;

out vec4 color;

uniform sampler2D texture_atlas;

void main() {


    vec4 temp_color = texture(texture_atlas, texture_atlas_id);

    vec3 magenta = vec3(1.0, 0.0, 1.0);

    float threshold = 0.25;

    float diff = distance(temp_color.rgb, magenta);

    if (diff < threshold) {
        discard;
    }

    color = temp_color;

}
