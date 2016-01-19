#version 330 core

in vec3 color;
in vec2 texCoord0;

uniform sampler2D texture;

out vec4 fragColor;

void main(void) {
	fragColor = texture2D(texture, texCoord0).rgba;
}
