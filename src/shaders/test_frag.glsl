#version 150

out vec4 fragColor;
in vec3 color;

void main() {
    fragColor = vec4(color, 1.0);
}
