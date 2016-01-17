#version 140

uniform mat4 mvp;

in vec3 position;
in vec3 inColor;

out vec3 color;

void main() {
    gl_Position = mvp * vec4(position, 1.0f);
	color = inColor;
}
