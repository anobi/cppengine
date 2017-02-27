#version 330 core

const int numLights = 5;

out VertexOut {

	vec2 texCoords;
	
	vec3 fragPos;
	vec3 tFragPos;

	vec3 viewPos;
	vec3 viewDir;

	vec3 tViewPos;
	vec3 tViewDir;

	mat3 tbn;
} vs_out;

struct Light {
	vec3 direction;
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
uniform Light Lights[numLights];

out vec3 tLightPos[numLights];
out vec3 tLightDir[numLights];

void main()
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0f);

	vec3 T		= normalize((ModelMatrix * vec4(tangent, 0.0f)).xyz);
	vec3 N		= normalize((ModelMatrix * vec4(normal, 0.0f)).xyz);
	T           = normalize(T - dot(T, N) * N);
	vec3 B		= cross(T, N);
	mat3 TBN	= transpose(mat3(T, B, N));

	vs_out.texCoords	= texCoord;
	vs_out.fragPos		= vec3(ModelMatrix * vec4(position, 1.0f));
	vs_out.viewPos		= CameraPosition;
	vs_out.viewDir		= CameraPosition - position;

	vs_out.tbn			= TBN;
	vs_out.tFragPos		= TBN * vs_out.fragPos;
	vs_out.tViewPos		= TBN * vs_out.viewPos;
	vs_out.tViewDir		= vs_out.tViewPos - vs_out.tFragPos;

	for(int i = 0; i < numLights; i++)
	{
		tLightPos[i] = TBN * Lights[i].position;
		tLightDir[i] = TBN * (Lights[i].position - vs_out.fragPos);
	}
}
