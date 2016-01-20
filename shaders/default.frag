#version 330 core

in vec2 texCoord0;
in vec3 normal0;

uniform sampler2D texture_diffuse;
uniform vec3 LightDirection;

out vec4 fragColor;

void main(void) {
	fragColor = texture(texture_diffuse, texCoord0);
}
