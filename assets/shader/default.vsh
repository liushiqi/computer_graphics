#version 330 core

in vec2 position;
in vec2 coordinate;

out vec2 coordinate1;

void main() {
    gl_Position = vec4(position, 0.0f, 1.0f);
    coordinate1 = coordinate;
}
