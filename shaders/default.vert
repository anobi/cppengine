#version 330 core

const int numLights = 5;

out VertexOut {

	vec2 texCoords;
	
	vec3 fragPos;
	vec3 tFragPos;
	vec3 viewPos;
	vec3 viewDir;

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

layout (location = 1) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in vec2 texCoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform vec3 CameraPosition;
uniform Light Lights[numLights];



out vec3 LightPos[numLights];
out vec3 LightDir[numLights];

void main() {

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0f);

	mat3 normalMatrix = transpose(inverse(mat3(ModelMatrix)));
	vec3 T		= vec3(normalMatrix * tangent);
	vec3 B		= vec3(normalMatrix * bitangent);
	vec3 N		= vec3(normalMatrix * normal);
	mat3 TBN	= transpose(mat3(T, B, N));

	vs_out.tbn			= TBN;
	vs_out.texCoords	= texCoord;
	vs_out.fragPos		= vec3(ModelMatrix * vec4(position, 1.0f));
	vs_out.tFragPos		= TBN * vs_out.fragPos;
	vs_out.viewPos		= TBN * CameraPosition;
	vs_out.viewDir		= normalize((TBN * CameraPosition) - (TBN * position));

	for(int i = 0; i < numLights; i++){
		LightPos[i] = TBN * Lights[i].position;
		LightDir[i] = normalize(LightPos[i] - vs_out.tFragPos);
	}

}
