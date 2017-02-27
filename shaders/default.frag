#version 330 core

const int numPLights = 5;
const int numDLights = 3;

in VertexOut {

	vec2 texCoords;

	vec3 normal;
	
	vec3 fragPos;
	vec3 tFragPos;

	vec3 viewPos;
	vec3 viewDir;

	vec3 tViewPos;
	vec3 tViewDir;

	mat3 tbn;
} vs_in;

struct pointLight {
	vec3 direction;
	vec3 position;
	vec3 color;
	float intensity;
	float radius;
	float cutoff;
};
uniform pointLight pointLights[numPLights];
in vec3 tPLightPos[numPLights];
in vec3 tPLightDir[numPLights];

struct directionalLight {
	vec3 position;
	vec3 color;
	float intensity;
};
uniform directionalLight directionalLights[numDLights];
in vec3 dLightDir[numDLights];
in vec3 tDLightDir[numDLights];

uniform int Time;
uniform vec2 Resolution;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform sampler2D heightMap;
uniform sampler2D emissiveMap;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 CameraPosition;

out vec4 fragColor;


float lambert(vec3 L, vec3 N) {
	return max(dot(L, N), 0.0f);
}

vec3 specular(vec3 L, vec3 N, vec3 V) 
{
	vec3 value = vec3(0.0f);

	if(max(dot(L, N), 0.0f) > 0.0f)
	{
		//phong:
		vec3 specularity = vec3(1.0f, 1.0f, 1.0f);
		float specular_hardness = 32.0f;
		float specular_intensity = 0.5f;

		//H = halfway vector
		vec3 H = normalize(L + V);
		float spec = pow(max(dot(N, H), 0.0f), specular_hardness);

		value = specularity * specular_intensity * spec;
	}
	return value;
}

vec3 addDirectionalLight(int index, vec2 texCoords, vec3 viewDir)
{
	directionalLight light = directionalLights[index];
	vec3 value = vec3(0.0f);
	vec3 L = normalize(vec3(0.5f, 0.5f, 0.2f));
	vec3 N = normalize(vs_in.normal);

	value += light.color * lambert(L, N);
	value += light.color * specular(L, N, viewDir);
	return value * light.intensity;
}

vec3 addPointLight(int index, vec2 texCoords, vec3 viewDir) {

	pointLight light = pointLights[index];

	vec3 value = vec3(0.0f);
	float d = distance(tPLightPos[index], vs_in.tFragPos);
	float dr = (max(d - light.radius, 0.0f) / light.radius) + 1.0f;
	
	vec3 L = normalize(tPLightDir[index]);
	vec3 N = normalize(texture(normalMap, texCoords).xyz * 2.0f - 1.0f);

	//attenuation
	float attenuation = 1.0f / (dr * dr);
	attenuation = (attenuation - light.cutoff) / (1.0f - light.cutoff);
	attenuation = max(attenuation, 0.0f);

	value += light.color * lambert(L, N);
	value += light.color * specular(L, N, viewDir);
	return value * attenuation;
}

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir, float scale, out float parallaxHeight) 
{
	const float minLayers = 10.0f;
	const float maxLayers = 20.0f;
	float layers = 20.0f; //mix(maxLayers, minLayers, abs(dot(vec3(0.0f, 0.0f, 1.0f), viewDir)));

	float depth = 1.0f / layers;
	float currentDepth = 0.0f;

	vec2 offset = texCoords;
	float heightValue = texture(heightMap, offset).r;

	vec2 delta = scale * viewDir.xy / -viewDir.z / layers;

	while(heightValue > currentDepth)
	{
		currentDepth += depth;
		offset -= delta;
		heightValue = texture(heightMap, offset).r;
	}
	
	vec2 prevTexCoords = offset + delta;
	float nextHeight = heightValue - currentDepth;
	float prevHeight = texture(heightMap, prevTexCoords).r - currentDepth + depth;
	float weight = nextHeight / (nextHeight - prevHeight);
	vec2 finalTexCoords = prevTexCoords * weight + offset * (1.0f - weight);

	parallaxHeight = currentDepth + prevHeight * weight + nextHeight * (1.0f - weight);

    return finalTexCoords;
}

vec2 par(vec2 texCoords, vec3 viewDir)
{ 
    float height = texture(heightMap, vs_in.texCoords).r;
    float p = height * 0.04f;
    return texCoords + viewDir.xy * p;    
}

void main(void)
{
	vec4 light = vec4(0.2f);
	vec3 viewDir = normalize(vs_in.tViewDir);
	vec2 texCoords = vs_in.texCoords;
	float parallaxHeight = 1.0f;

	//texCoords = parallaxMapping(texCoords, viewDir, 0.02f, parallaxHeight);

	if(texCoords.x > 1.0f || texCoords.y > 1.0f || texCoords.x < 0.0f || texCoords.y < 0.0f)
	{
		//discard;
	}

	for(int i = 0; i < numDLights; i++)
	{
		light += vec4(addDirectionalLight(i, texCoords, viewDir), 0.0f);
	}
	
	//light position translated to camera space
	for(int i = 0; i < numPLights; i++)
	{
		//light += vec4(addPointLight(i, texCoords, viewDir), 1.0f) * pointLights[i].intensity;
	}

	fragColor = texture(diffuseMap, texCoords) * light;
}
