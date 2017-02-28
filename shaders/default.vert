#version 330 core

const int numPLights = 5;
const int numDLights = 3;

out VertexOut {

	vec2 texCoords;
	
	vec3 normal;
	
	vec3 fragPos;
	vec3 tFragPos;

	vec3 viewPos;
	vec3 viewDir;

	vec3 tViewPos;
	vec3 tViewDir;

	mat3 tbn;
} vs_out;

struct directionalLight {
	vec3 position;
	vec3 color;
	float intensity;
};

struct pointLight {
	vec3 position;
	vec3 color;
	float intensity;
	float radius;
	float cutoff;
};

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 texCoord;

uniform int Time;
uniform vec2 Resolution;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;
uniform vec3 CameraPosition;

uniform pointLight pointLights[numPLights];
uniform directionalLight directionalLights[numDLights];

out vec3 PLightPos[numPLights];
out vec3 DLightPos[numDLights];

void main()
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0f);

	vec3 T		= normalize((ModelMatrix * vec4(tangent, 0.0f)).xyz);
	vec3 N		= normalize((ModelMatrix * vec4(normal, 0.0f)).xyz);
	T           = normalize(T - dot(T, N) * N);
	vec3 B		= cross(T, N);
	mat3 TBN	= transpose(mat3(T, B, N));

	vs_out.texCoords	= texCoord;
	vs_out.normal       = mat3(transpose(inverse(ViewMatrix * ModelMatrix))) * normal;
	vs_out.fragPos		= vec3(ViewMatrix * ModelMatrix * vec4(position, 1.0f));

	for(int i = 0; i < numPLights; i++)
	{
		PLightPos[i] = vec3(ViewMatrix * vec4(pointLights[i].position, 1.0));
	}

	for(int i = 0; i < numDLights; i++)
	{
		DLightPos[i] = vec3(ViewMatrix * vec4(directionalLights[i].position, 1.0));
	}
}
