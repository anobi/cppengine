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

out vec3 tLightPos[numLights];
out vec3 tLightDir[numLights];

uniform sampler2D HeightMap;

void main() {

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0f);

	mat3 normalMatrix = transpose(inverse(mat3(ModelMatrix)));
	vec3 T		= normalize(vec3(normalMatrix * tangent));
	vec3 B		= normalize(vec3(normalMatrix * bitangent));
	vec3 N		= normalize(vec3(normalMatrix * normal));
	mat3 TBN	= transpose(mat3(T, B, N));

	vs_out.tbn			= TBN;
	vs_out.texCoords	= texCoord;

	vs_out.fragPos		= vec3(ModelMatrix * vec4(position, 1.0f));
	vs_out.viewPos		= CameraPosition;
	vs_out.viewDir		= normalize(CameraPosition - position);

	vs_out.tFragPos		= TBN * vs_out.fragPos;
	vs_out.tViewPos		= TBN * CameraPosition;
	vs_out.tViewDir		= normalize(vs_out.tViewPos - vs_out.tFragPos);

	for(int i = 0; i < numLights; i++){
		tLightPos[i] = TBN * Lights[i].position;
		tLightDir[i] = TBN * (Lights[i].position - vs_out.fragPos);
	}

}
