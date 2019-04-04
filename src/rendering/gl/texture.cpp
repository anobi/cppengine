#include "opengl.hpp"
#include "../../material.hpp"
#include "texture.hpp"

Rendering::Texture::Texture(std::shared_ptr<Resources::Texture> texture)
{
	glGenTextures(1, &texture->GetId());
	glBindTexture(GL_TEXTURE_2D, texture->_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

Rendering::Texture::~Texture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &this->_id);
}

void Rendering::Texture::Draw(GLuint shader_program)
{
	glActiveTexture(GL_TEXTURE0 + this->_i);
	glUniform1i(glGetUniformLocation(shader_program, (this->_type + "Map").c_str()), this->_i);
	glUniform1i(glGetUniformLocation(shader_program, ("use_" + this->_type + "Map").c_str()), 1);
	glBindTexture(GL_TEXTURE_2D, this->_id);
}

void Rendering::Texture::PostDraw(GLuint shader_program)
{
	glUniform1i(glGetUniformLocation(shader_program, (this->_type + "Map").c_str()), 0);
	glUniform1i(glGetUniformLocation(shader_program, ("use_" + this->_type + "Map").c_str()), 0);

	glActiveTexture(GL_TEXTURE0);
}
