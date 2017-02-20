#version 330 core

in VertexOut {
	vec2 texCoords;
} vs_in;

uniform int Time;
uniform vec2 Resolution;

out vec4 fragColor;

void main(void) {

	vec2 texCoords = vs_in.texCoords * Time;

	float rand = fract(sin(dot(texCoords, vec2(12.9898f, 4.1414f))) * 43758.5453f);
	vec3 color = vec3(rand);

	fragColor = vec4(color, 1.0f);
}
