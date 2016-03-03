#version 330 core

in vec2 texCoord0;
in vec3 normal0;
in vec3 position0;

in vec3 eyePos;
in vec3 eyeDir;
in vec3 worldPos;

uniform sampler2D AlbedoMap;
uniform sampler2D MetallicMap;
uniform sampler2D NormalMap;

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

out vec4 fragColor;

float lambert(vec3 N, vec3 L) {
	return clamp(dot(N, L), 0.0f, 1.0f);
}

vec3 specular(vec3 L, vec3 N) {
	vec3 value = vec3(0.0f);

	if(dot(L, N) > 0.0f){

		//phong:
		vec3 specularity = vec3(1.0f, 1.0f, 1.0f);
		float specular_hardness = 60.0f;
		float specular_intensity = 0.5f;

		//E = direction vector from vertex to eye
		vec3 E = normalize(eyeDir); 
		vec3 R = reflect(-L, N);
		float spec = pow(max(dot(R, E), 0.0f), specular_hardness);

		value = specularity * specular_intensity * spec;
	}
	return value;
}

float attenuation(float distance){
	return clamp(1.0f / distance * distance, 0.0f, 1.0f);
}

vec3 pointLight(vec3 position, vec3 color, float radius, float cutoff) {
	vec3 value = vec3(0.0f);
	float d = distance(position, position0);
	float dr = (max(d - radius, 0.0f) / radius) + 1.0f;
	vec3 L = normalize(position + eyeDir);	//L = direction from fragment to camera
	vec3 N = normalize(2.0f * texture(NormalMap, texCoord0).rgb - 1.0f);			//N = vertex normal in camera space

	//attenuation

	float attenuation = 1.0f / (dr * dr);
	attenuation = (attenuation - cutoff) / (1.0f - cutoff);
	attenuation = max(attenuation, 0.0f);

	value += color * lambert(N, L);
	value += color * specular(L, N);
	return value * attenuation;
}

void main(void) {

	vec4 light = vec4(0.0f);

	//light position translated to camera space
	for(int i = 0; i < numLights; i++){
		vec3 lPos = (ViewMatrix * vec4(Lights[i].position, 1.0f)).xyz;
		light += vec4(pointLight(lPos, Lights[i].color, Lights[i].radius, Lights[i].cutoff), 1.0f) * Lights[i].intensity;
	}

	fragColor = texture(AlbedoMap, texCoord0) * light;
}
