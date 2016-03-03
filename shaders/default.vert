#version 330 core
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec2 texCoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 CameraPosition;

struct Light {
	vec3 direction;
	vec3 position;
	vec3 color;
	float intensity;
	float radius;
	float cutoff;
};

const int numLights = 5;
uniform Light Lights[numLights];

out vec2 texCoord0;
out vec3 normal0;
out vec3 position0;
out vec3 tangent0;

out vec3 eyePos;
out vec3 eyeDir;
out vec3 worldPos;

out Light lights0[numLights];

void main() {

	mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
    gl_Position = MVP * vec4(position, 1.0f);

	texCoord0 = texCoord;
	normal0 = (ViewMatrix * ModelMatrix * vec4(normal, 0.0f)).xyz;
	position0 = (ViewMatrix * ModelMatrix * vec4(position, 1.0f)).xyz;
	tangent0 = (ViewMatrix * ModelMatrix * vec4(tangent, 0.0f)).xyz;

	worldPos = (ModelMatrix * vec4(position, 1.0f)).xyz;
	eyePos = CameraPosition;
	eyeDir = vec3(0.0f) - position0;

	for(int i = 0; i < numLights; i++){
		lights0[i].position = (ViewMatrix * vec4(Lights[i].position, 1.0f)).xyz;
		lights0[i].direction = normalize(position + eyeDir);
		lights0[i].color = Lights[i].color;

		lights0[i].intensity = Lights[i].color;
		lights0[i].radius = Lights[i].color;
		lights0[i].cutoff = Lights[i].color;
	}
}
