#version 330 core

in vec2 coordinate1;

uniform sampler2D texture1;

out vec4 color_result;

void main() {
    color_result = texture(texture1, coordinate1);
}
