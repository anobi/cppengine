#version 150

in vec3 position;

void main (){
     gl_Location = vec4(position, 1.0);
}