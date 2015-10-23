#include <iostream>
#include "Entity.hpp"

#include "lib/Vector.hpp"
#include "lib/OpenGL.hpp"

Entity::Entity(){
	location.Zero();
	alive = false;
}

Entity::~Entity(){
	
}

void Entity::Init() {
    MakeTestEntity();
    renderEntity.shader = LoadShader("shaders/test_vert.glsl"
                                   , "shaders/test_frag.glsl");
}

void Entity::Spawn() {
	
}

void Entity::Update() {
}

void Entity::Kill() {
	
}

void Entity::Remove() {
	
}

vector3<float> Entity::GetLocation() {
	return location;
}

void Entity::SetLocation(float x, float y, float z) {
	location.Set(x, y, z);
}

//manual test cube mesh
void Entity::MakeTestEntity() {

	//
	//left side
	//
	vertex_t l1, l2, l3, l4, l5, l6 = vertex_t();

	l1.position = vector3<float>(0.0f, 0.0f, 0.0f);
	l2.position = l4.position = vector3<float>(0.0f, 0.0f, 0.5f);
	l3.position = l6.position = vector3<float>(0.0f, 0.5f, 0.0f);
	l5.position = vector3<float>(0.0f, 0.5f, 0.5f);

	l1.color = l2.color = l3.color = l4.color = l5.color = l6.color = vector3<float>(1.0f, 0.0f, 0.0f);

	renderEntity.vertices.push_back(l1);
	renderEntity.vertices.push_back(l2);
	renderEntity.vertices.push_back(l3);
	renderEntity.vertices.push_back(l4);
	renderEntity.vertices.push_back(l5);
	renderEntity.vertices.push_back(l6);
	
	//
	//front side
	//
	vertex_t f1, f2, f3, f4, f5, f6 = vertex_t();

	f1.position = vector3<float>(0.0f, 0.0f, 0.5f);
	f2.position = f4.position = vector3<float>(0.5f, 0.0f, 0.5f);
	f3.position = f6.position = vector3<float>(0.0f, 0.5f, 0.5f);
	f5.position = vector3<float>(0.5f, 0.5f, 0.5f);

	f1.color = f2.color = f3.color = f4.color = f5.color = f6.color = vector3<float>(0.0f, 1.0f, 0.0f);

	renderEntity.vertices.push_back(f1);
	renderEntity.vertices.push_back(f2);
	renderEntity.vertices.push_back(f3);
	renderEntity.vertices.push_back(f4);
	renderEntity.vertices.push_back(f5);
	renderEntity.vertices.push_back(f6);

	//
	//right side
	//
	vertex_t r1, r2, r3, r4, r5, r6 = vertex_t();

	r1.position = vector3<float>(0.5f, 0.0f, 0.5f);
	r2.position = r4.position = vector3<float>(0.5f, 0.0f, 0.0f);
	r3.position = r6.position = vector3<float>(0.5f, 0.5f, 0.0f);
	r5.position = vector3<float>(0.5f, 0.5f, 0.5f);

	r1.color = r2.color = r3.color = 
	r4.color = r5.color = r6.color = vector3<float>(0.0f, 0.0f, 1.0f);

	renderEntity.vertices.push_back(r1);
	renderEntity.vertices.push_back(r2);
	renderEntity.vertices.push_back(r3);
	renderEntity.vertices.push_back(r4);
	renderEntity.vertices.push_back(r5);
	renderEntity.vertices.push_back(r6);
	
	//backside
	vertex_t b1, b2, b3, b4, b5, b6 = vertex_t();

	b1.position = vector3<float>(0.0f, 0.0f, 0.0f);
	b2.position = b4.position = vector3<float>(0.5f, 0.0f, 0.0f);
	b3.position = b6.position = vector3<float>(0.0f, 0.5f, 0.0f);
	b5.position = vector3<float>(0.5f, 0.5f, 0.0f);

	b1.color = b2.color = b3.color = 
	b4.color = b5.color = b6.color = vector3<float>(0.0f, 1.0f, 1.0f);

	renderEntity.vertices.push_back(b1);
	renderEntity.vertices.push_back(b2);
	renderEntity.vertices.push_back(b3);
	renderEntity.vertices.push_back(b4);
	renderEntity.vertices.push_back(b5);
	renderEntity.vertices.push_back(b6);

	//topside
	vertex_t t1, t2, t3, t4, t5, t6 = vertex_t();

	t1.position = vector3<float>(0.0f, 0.5f, 0.0f);
	t2.position = t4.position = vector3<float>(0.5f, 0.5f, 0.0f);
	t3.position = t6.position = vector3<float>(0.0f, 0.5f, 0.5f);
	t5.position = vector3<float>(0.5f, 0.5f, 0.5f);

	t1.color = t2.color = t3.color = 
	t4.color = t5.color = t6.color = vector3<float>(1.0f, 1.0f, 1.0f);

	renderEntity.vertices.push_back(t1);
	renderEntity.vertices.push_back(t2);
	renderEntity.vertices.push_back(t3);
	renderEntity.vertices.push_back(t4);
	renderEntity.vertices.push_back(t5);
	renderEntity.vertices.push_back(t6);

	//downside lol
	vertex_t d1, d2, d3, d4, d5, d6 = vertex_t();

	d1.position = vector3<float>(0.0f, 0.0f, 0.0f);
	d2.position = d4.position = vector3<float>(0.5f, 0.0f, 0.0f);
	d3.position = d6.position = vector3<float>(0.0f, 0.0f, 0.5f);
	d5.position = vector3<float>(0.5f, 0.0f, 0.5f);

	d1.color = d2.color = d3.color =
	d4.color = d5.color = d6.color = vector3<float>(1.0f, 1.0f, 0.0f);

	renderEntity.vertices.push_back(d1);
	renderEntity.vertices.push_back(d2);
	renderEntity.vertices.push_back(d3);
	renderEntity.vertices.push_back(d4);
	renderEntity.vertices.push_back(d5);
	renderEntity.vertices.push_back(d6);
}

//have these here temporarily, gotta make glutils or something
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

std::string Entity::ReadFile(const char* filePath) {
    std::string content;
    std::ifstream stream(filePath, std::ios::in);

    if(!stream.is_open()) {
        std::cout << "Cannot read file " << filePath << std::endl;
        return "";
    }

    std::string line = "";
    while(!stream.eof()) {
        std::getline(stream, line);
        content.append(line + "\n");
    }

    stream.close();
    return content;
}

GLuint Entity::LoadShader(const char* vertex_shader_path, const char* fragment_shader_path) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertexShaderStr = ReadFile(vertex_shader_path);
    std::string fragmentShaderStr = ReadFile(fragment_shader_path);
    const char* vertexShaderSrc = vertexShaderStr.c_str();
    const char* fragmentShaderSrc = fragmentShaderStr.c_str();

    int logLength = 0;
    GLint result = GL_FALSE;

    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> vertexShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(vertexShader, logLength, NULL, &vertexShaderError[0]);
    std::cout << &vertexShaderError[0] << std::endl;
    
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> fragmentShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(fragmentShader, logLength, NULL, &fragmentShaderError[0]);
    std::cout << &fragmentShaderError[0] << std::endl;

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glBindFragDataLocation(program, 0, "outColor");
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> programError( (logLength > 1) ? logLength : 1 );
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    std::cout << &programError[0] << std::endl;
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}
