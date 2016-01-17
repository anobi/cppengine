#version 140

uniform mat4 modelViewProjection;
uniform mat4 wNormal;
uniform vec3 diffuse;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec3 color;

void main() {
    gl_Position = modelViewProjection * vec4(position, 1.0f);

	color = diffuse;
}
