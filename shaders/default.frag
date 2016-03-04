#version 330 core

const int numLights = 5;

in VertexOut {

	vec2 texCoords;
	
	vec3 fragPos;
	vec3 tFragPos;
	vec3 viewPos;
	vec3 viewDir;

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
in vec3 LightPos[numLights];
in vec3 LightDir[numLights];

uniform sampler2D AlbedoMap;
uniform sampler2D MetallicMap;
uniform sampler2D NormalMap;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 CameraPosition;

out vec4 fragColor;

float lambert(vec3 L, vec3 N) {
	return clamp(dot(L, N), 0.0f, 1.0f);
}

vec3 specular(vec3 L, vec3 N) {
	vec3 value = vec3(0.0f);

	if(dot(L, N) > 0.0f){

		//phong:
		vec3 specularity = vec3(1.0f, 1.0f, 1.0f);
		float specular_hardness = 32.0f;
		float specular_intensity = 0.5f;

		//E = direction vector from vertex to eye
		vec3 E = normalize(L + vs_in.viewDir); 
		vec3 R = reflect(-L, N);
		float spec = pow(max(dot(R, E), 0.0f), specular_hardness);

		value = specularity * specular_intensity * spec;
	}
	return value;
}

float attenuation(float distance){
	return clamp(1.0f / distance * distance, 0.0f, 1.0f);
}

vec3 pointLight(vec3 position, vec3 direction, vec3 color, float radius, float cutoff) {
	vec3 value = vec3(0.0f);
	float d = distance(position, vs_in.fragPos);
	float dr = (max(d - radius, 0.0f) / radius) + 1.0f;
	
	vec3 L = direction;
	vec3 N = normalize(2.0f * texture(NormalMap, vs_in.texCoords).rgb - 1.0f);			//N = vertex normal in camera space
	//N = normalize(vs_in.tbn * N);

	//attenuation

	float attenuation = 1.0f / (dr * dr);
	attenuation = (attenuation - cutoff) / (1.0f - cutoff);
	attenuation = max(attenuation, 0.0f);

	value += color * lambert(L, N);
	value += color * specular(L, N);
	return value * attenuation;
}

void main(void) {

	vec4 light = vec4(0.0f);

	//light position translated to camera space
	for(int i = 0; i < numLights; i++){
		light += vec4(pointLight(LightPos[i], LightDir[i], Lights[i].color, Lights[i].radius, Lights[i].cutoff), 1.0f) * Lights[i].intensity;
	}

	fragColor = texture(AlbedoMap, vs_in.texCoords) * light;
}
