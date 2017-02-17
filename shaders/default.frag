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

float lambert(vec3 L, vec3 N) {
	return max(dot(L, N), 0.0f);
}

vec3 specular(vec3 L, vec3 N, vec3 V) {
	vec3 value = vec3(0.0f);

	if(max(dot(L, N), 0.0f) > 0.0f){

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

vec3 pointLight(int index, vec2 texCoords, vec3 viewDir) {

	Light light = Lights[index];

	vec3 value = vec3(0.0f);
	float d = distance(tLightPos[index], vs_in.tFragPos);
	float dr = (max(d - light.radius, 0.0f) / light.radius) + 1.0f;
	
	vec3 L = normalize(tLightDir[index]);
	vec3 N = normalize(texture(NormalMap, texCoords).xyz * 2.0f - 1.0f);

	//attenuation
	float attenuation = 1.0f / (dr * dr);
	attenuation = (attenuation - light.cutoff) / (1.0f - light.cutoff);
	attenuation = max(attenuation, 0.0f);

	value += light.color * lambert(L, N);
	value += light.color * specular(L, N, viewDir);
	return value * attenuation;
}

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir, float scale, out float parallaxHeight) {

	const float minLayers = 10.0f;
	const float maxLayers = 15.0f;
	float layers = mix(maxLayers, minLayers, abs(dot(vec3(0.0f, 0.0f, 1.0f), viewDir)));

	float layerHeight = 1.0f / layers;
	float currentLayerHeight = 0.0f;
	vec2 dTexCoords = scale * viewDir.xy / viewDir.z / layers;

	vec2 currentTexCoords = texCoords;
	float heightFromTexture = texture(HeightMap, currentTexCoords).r;
	float prevHeightFromTexture = 0.0f;

	while(currentLayerHeight < heightFromTexture
		  && prevHeightFromTexture != heightFromTexture)
	{
		currentLayerHeight += layerHeight;
		currentTexCoords -= dTexCoords;
		prevHeightFromTexture = heightFromTexture;
		heightFromTexture = texture(HeightMap, currentTexCoords).r;
	}

	vec2 prevTexCoords = currentTexCoords + dTexCoords;
	float nextHeight = heightFromTexture - currentLayerHeight;
	float prevHeight = texture(HeightMap, prevTexCoords).r - currentLayerHeight + layerHeight;

	float weight = nextHeight / (nextHeight - prevHeight);

	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0f - weight);

	parallaxHeight = currentLayerHeight + prevHeight * weight + nextHeight * (1.0f - weight);

    return finalTexCoords;  
}

vec2 par(vec2 texCoords, vec3 viewDir)
{ 
    float height =  texture(HeightMap, texCoords).r;    
    vec2 p = viewDir.xy / viewDir.z * (height * 0.1f);
    return texCoords - p;    
}

void main(void)
{
	vec4 light = vec4(0.0f);
	vec3 viewDir = vs_in.tViewDir;
	vec2 texCoords = vs_in.texCoords;
	float parallaxHeight = 1.0f;

	// Problem seems to be calculating shit from vertex center, not from camera POV
	// so per pixel view dir or improve tesselation?
	//texCoords = parallaxMapping(texCoords, viewDir, 0.2f, parallaxHeight);

	//light position translated to camera space
	for(int i = 0; i < numLights; i++){
		light += vec4(pointLight(i, texCoords, viewDir), 1.0f) * Lights[i].intensity;
	}

	fragColor = texture(AlbedoMap, texCoords) * light;
}
