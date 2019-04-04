#ifndef R_GL_MESH_H
#define R_GL_MESH_H

#include "../renderable.hpp"
#include "../../mesh.hpp"
#include "texture.hpp"
#include "opengl.hpp"

namespace Rendering {
	class Mesh : Renderable {
	public:
		Mesh(std::shared_ptr<Resources::Mesh>);
		~Mesh();
		void Draw();
		void PostDraw();

	private:
		std::vector<std::shared_ptr<Rendering::Texture>> _textures;

		GLuint _vao = (GLuint)0;
		GLuint _vbo = (GLuint)0;
		GLuint _ebo = (GLuint)0;

		unsigned int _num_indices = 0;
		unsigned int _num_vertices = 0;
	};
}

#endif