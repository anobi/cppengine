#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main() 
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0f);
}
