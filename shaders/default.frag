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
uniform sampler2D MetallicMap;
uniform sampler2D NormalMap;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 CameraPosition;

out vec4 fragColor;

float lambert(vec3 L, vec3 N) {
	return max(dot(L, N), 0.0f);
}

vec3 specular(vec3 L, vec3 N) {
	vec3 value = vec3(0.0f);

	if(max(dot(L, N), 0.0f) > 0.0f){

		//phong:
		vec3 specularity = vec3(1.0f, 1.0f, 1.0f);
		float specular_hardness = 32.0f;
		float specular_intensity = 0.5f;

		//H = halfway vector
		vec3 H = normalize(L + vs_in.tViewDir);
		float spec = pow(max(dot(N, H), 0.0f), specular_hardness);

		value = specularity * specular_intensity * spec;
	}
	return value;
}

vec3 pointLight(int index) {

	Light light = Lights[index];

	vec3 value = vec3(0.0f);
	float d = distance(tLightPos[index], vs_in.tFragPos);
	float dr = (max(d - light.radius, 0.0f) / light.radius) + 1.0f;
	
	vec3 L = normalize(tLightDir[index]);
	vec3 N = texture(NormalMap, vs_in.texCoords).xyz;
	N = normalize(N * 2.0f - 1.0f);

	//attenuation
	float attenuation = 1.0f / (dr * dr);
	attenuation = (attenuation - light.cutoff) / (1.0f - light.cutoff);
	attenuation = max(attenuation, 0.0f);

	value += light.color * lambert(L, N);
	value += light.color * specular(L, N);
	return value * attenuation;
}

void main(void) {

	vec4 light = vec4(0.0f);

	//light position translated to camera space
	for(int i = 0; i < numLights; i++){
		light += vec4(pointLight(i), 1.0f) * Lights[i].intensity;
	}

	fragColor = texture(AlbedoMap, vs_in.texCoords) * light;
}
