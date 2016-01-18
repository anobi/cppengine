#version 150

uniform mat4 modelViewProjection;
uniform mat4 wNormal;
uniform vec3 diffuse;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec3 color;
out vec2 texCoord0;
out vec3 normal0;

void main() {
    gl_Position = modelViewProjection * vec4(position, 1.0f);
	texCoord0 = texCoord;
	normal0 = (wNormal * vec4(normal, 0.0)).xyz;
	color = diffuse;
}
