#version 140

in vec3 position;
in vec3 color;

out vec3 color_out;

void main (void){
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    color_out = color;
}
