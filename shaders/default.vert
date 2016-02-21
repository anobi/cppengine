#version 330 core
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texCoord;

uniform mat4 ModelViewProjection;
uniform mat4 Normal;

out vec2 texCoord0;
out vec3 normal0;
out vec3 position0;

void main() {
    gl_Position = ModelViewProjection * vec4(position, 1.0f);
	texCoord0 = texCoord;
	normal0 = (Normal * vec4(normal, 0.0)).xyz;
	position0 = position;
}
