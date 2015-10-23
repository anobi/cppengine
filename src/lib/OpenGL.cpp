#include "OpenGL.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

std::string GLUtils::ReadFile(const char* filePath) {
    std::string content;
    std::ifstream stream(filePath, std::ios::in);

    if(!stream.is_open()) {
        std::cerr << "Cannot read file " << filePath << std::endl;
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

GLuint GLUtils::LoadShader(const char* vertex_shader_path, const char* fragment_shader_path) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertShaderStr = ReadFile(vertex_shader_path);
    std::string fragShaderStr = ReadFile(fragment_shader_path);
    const char* vertShaderSrc = vertShaderStr.c_str();
    const char* fragShaderSrc = fragShaderStr.c_str();

    glShaderSource(vertexShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragmentShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}
