#version 330 core
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texCoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 CameraPosition;

out vec2 texCoord0;
out vec3 normal0;
out vec3 position0;

out vec3 eyePos;
out vec3 eyeDir;
out vec3 worldPos;

void main() {

	mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
    gl_Position = MVP * vec4(position, 1.0f);

	texCoord0 = texCoord;
	normal0 = (ViewMatrix * ModelMatrix * vec4(normal, 0.0f)).xyz;
	position0 = (ViewMatrix * ModelMatrix * vec4(position, 1.0f)).xyz;

	worldPos = (ModelMatrix * vec4(position, 1.0f)).xyz;
	eyePos = CameraPosition;
	eyeDir = vec3(0.0f) - position0;
}
