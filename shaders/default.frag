#version 330 core

in vec2 texCoord0;
in vec3 normal0;

uniform sampler2D texture_diffuse;

uniform vec3 LightDirection;
uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform float LightIntensity;
uniform float LightMaxDistance;

out vec4 fragColor;

void main(void) {

	vec4 ambientLight = vec4(0.0f, 0.0f, 0.1f, 1.0f);
	vec4 lightColor = vec4(LightColor, 1.0f);
	float light = clamp(dot(normal0, LightDirection), 0.0f, 1.0f);

	fragColor = texture(texture_diffuse, texCoord0) * light * lightColor;
}
