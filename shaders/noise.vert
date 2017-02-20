#version 330 core

out VertexOut {
	vec2 texCoords;
} vs_out;

layout (location = 0) in vec3 position;
layout (location = 4) in vec2 texCoord;


uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main() 
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0f);
	vs_out.texCoords = texCoord;
}
