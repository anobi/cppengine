#version 330 core

in vec2 texCoord0;
in vec3 normal0;
in vec3 position0;

uniform sampler2D texture_diffuse;

uniform vec3 LightDirection;
uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform float LightIntensity;
uniform float LightMaxDistance;

out vec4 fragColor;

vec3 addSpecular(vec3 L, vec3 N) {
	vec3 value = vec3(0.0f);

	//specular choose your own adventure: phong or blinn-phong
	if(dot(L, N) > 0.0f){
		//phong:

		vec3 specularity = vec3(1.0f, 1.0f, 1.0f);
		float specular_hardness = 60.0f;
		float specular_intensity = 0.5f;

		vec3 E = normalize(-position0);
		vec3 R = reflect(-L, N);
		float spec = pow(max(dot(R, E), 0.0f), specular_hardness);

		//blinn-phong, faster but not as good looking:
		//vec3 H = normalize(lightDirection + L);
		//float spec = pow(max(dot(vNormal, H), 0.0f), specular_hardness); //the last bit is the shininess

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
	vec3 L = normalize(position - position0);
	vec3 N = normalize(normal0);
	//diffuse
	float lambert = clamp(dot(N, L), 0.0f, 1.0f);
	value += color * lambert;
	value += color * addSpecular(L, N);
	return value * attenuation(d);
}

void main(void) {

	vec4 light = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light += vec4(pointLight(LightPosition, LightColor), 1.0f);

	fragColor = texture(texture_diffuse, texCoord0) * light;
}
