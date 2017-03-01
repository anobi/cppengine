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
	vec3 position;
	vec3 color;
	float intensity;
	float radius;
	float cutoff;
};
uniform pointLight pointLights[numPLights];
in vec3 PLightPos[numPLights];

struct directionalLight {
	vec3 position;
	vec3 color;
	float intensity;
};
uniform directionalLight directionalLights[numDLights];
in vec3 DLightPos[numDLights];

uniform int Time;
uniform vec2 Resolution;

uniform int use_diffuseMap;
uniform int use_specularMap;
uniform int use_normalMap;
uniform int use_heightMap;
uniform int use_emissiveMap;
uniform int use_alphaMap;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform sampler2D heightMap;
uniform sampler2D emissiveMap;
uniform sampler2D alphaMap;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 CameraPosition;

out vec4 fragColor;


float diffuse(vec3 light_dir, vec3 normal)
{
	return max(dot(light_dir, normal), 0.0f);
}

vec3 specular(vec3 light_dir, vec3 normal, vec3 view_dir) 
{
	vec3 value = vec3(0.0f);
	vec3 specularity = vec3(1.0f, 1.0f, 1.0f);
	float specular_intensity = 1.0f;
	float shininess = 32.0f;

	if(use_specularMap == 1)
	{
		shininess = texture(specularMap, vs_in.texCoords).r * 255.0f;
	}

	if(shininess < 255.0f)
	{
		vec3 reflect_dir = reflect(-light_dir, normal);
		float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), shininess);
		value = specularity * spec;
	}

	return value;
}

vec3 addDirectionalLight(int index, vec2 texCoords, vec3 lightDir, vec3 viewDir, vec3 normal)
{
	directionalLight light = directionalLights[index];
	vec3 value = vec3(0.0f);

	value += light.color * diffuse(lightDir, normal);
	value += light.color * specular(lightDir, normal, viewDir);
	return value * light.intensity;
}

vec3 addPointLight(int index, vec2 texCoords, vec3 lightDir, vec3 viewDir, vec3 normal)
{
	pointLight light = pointLights[index];

	vec3 value = vec3(0.0f);
	float d = distance(PLightPos[index], vs_in.fragPos);
	float dr = (max(d - light.radius, 0.0f) / light.radius) + 1.0f;

	//attenuation
	float attenuation = 1.0f / (dr * dr);
	attenuation = (attenuation - light.cutoff) / (1.0f - light.cutoff);
	attenuation = max(attenuation, 0.0f);

	value += light.color * diffuse(lightDir, normal);
	value += light.color * specular(lightDir, normal, viewDir);
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
	vec3 light = vec3(0.2f);
	vec3 viewDir = normalize(-vs_in.fragPos);
	vec2 texCoords = vs_in.texCoords;


	//////////////////////
	//   Texture Maps   //
	//////////////////////

	float alpha = 1.0f;
	if(use_alphaMap == 1)
	{
		alpha = texture(alphaMap, texCoords).r;

		if(alpha < 0.8f)
		{
			discard;
		}
	}

	vec3 diffuse = vec3(0.8f, 0.8f, 0.8f);
	if(use_diffuseMap == 1)
	{
		diffuse = texture(diffuseMap, texCoords).rgb;
	}

	vec3 normal = normalize(vs_in.normal);
	if(use_normalMap == 1)
	{
		normal = normalize((texture(normalMap, vs_in.texCoords).rgb) * 2.0f - 1.0f);
	}

	if(use_heightMap == 1)
	{
		float parallaxHeight = 1.0f;
		texCoords = parallaxMapping(texCoords, viewDir, 0.02f, parallaxHeight);
	}


	//////////////////////
	// Calculate lights //
	//////////////////////

	// Directional lights
	for(int i = 0; i < numDLights; i++)
	{
		vec3 lightDir = normalize(DLightPos[i] - vs_in.fragPos);
		light += vec3(addDirectionalLight(i, texCoords, lightDir, viewDir, normal));
	}
	
	// Point lights
	for(int i = 0; i < numPLights; i++)
	{
		vec3 lightDir = normalize(PLightPos[i] - vs_in.fragPos);
		light += vec3(addPointLight(i, texCoords, lightDir, viewDir, normal)) * pointLights[i].intensity;
	}

	fragColor.rgb = diffuse * light;
	fragColor.a = alpha;
}
