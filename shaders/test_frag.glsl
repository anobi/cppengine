#version 140

in vec3 color;

void main() {
    gl_color = vec4(color, 1.0f);
}
