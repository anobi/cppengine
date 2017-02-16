#version 330 core

const int numLights = 5;

in VertexOut {
	vec2 texCoords;
	
	vec3 fragPos;
	vec3 tFragPos;

	vec3 viewPos;
	vec3 viewDir;

	vec3 tViewPos;
	vec3 tViewDir;

	mat3 tbn;
} vs_in;

struct Light {
	vec3 direction;
	vec3 position;
	vec3 color;
	float intensity;
	float radius;
	float cutoff;
};

uniform Light Lights[numLights];
in vec3 tLightPos[numLights];
in vec3 tLightDir[numLights];

uniform int Time;
uniform vec2 Resolution;

uniform sampler2D AlbedoMap;
uniform sampler2D NormalMap;
uniform sampler2D HeightMap;
uniform sampler2D RoughnessMap;
uniform sampler2D OcclusionMap;
uniform sampler2D MetallicMap;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 CameraPosition;

out vec4 fragColor;

void main(void) {

	vec2 texCoords = vs_in.texCoords / Resolution;

	float rand = fract(sin(dot(texCoords, Time)) * 1.0f);
	vec3 color = vec3(rand);

	fragColor = vec4(color, 1.0f);
}
