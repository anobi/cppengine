#version 330 core

in vec2 texCoord0;
in vec3 normal0;
in vec3 position0;

in vec3 eyePos;
in vec3 eyeDir;
in vec3 worldPos;

uniform sampler2D texture_diffuse;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 CameraPosition;

struct Light {
	vec3 direction;
	vec3 position;
	vec3 color;
	float intensity;
	float maxDistance;
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

vec3 pointLight(vec3 position, vec3 color) {
	vec3 value;
	float r = 69.0f;
	float d = distance(position, position0);

	vec3 L = normalize(position + eyeDir);	//L = direction from fragment to camera
	vec3 N = normalize(normal0);			//N = vertex normal in camera space

	value += color * lambert(N, L);
	value += color * specular(L, N);
	return value * attenuation(d);
}

void main(void) {

	vec4 light = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	//light position translated to camera space
	for(int i = 0; i < numLights; i++){
		vec3 lPos = (ViewMatrix * vec4(Lights[i].position, 1.0f)).xyz;
		light += vec4(pointLight(lPos, Lights[i].color), 1.0f);
	}

	fragColor = texture(texture_diffuse, texCoord0) * light;
}
