#ifndef R_GL_MESH
#define R_GL_MESH

#include "../render_entity.hpp"
#include "../../resources/res_mesh.hpp"
#include "gl_texture.hpp"
#include "opengl.hpp"

namespace Rendering {
	namespace GL {
		class GLMesh : public Rendering::RenderEntity {
		public:
			GLMesh(std::shared_ptr<Resources::Mesh>);
			~GLMesh();
			void Draw();
			void PostDraw();

		private:
			std::vector<std::shared_ptr<Rendering::GL::GLTexture>> _textures;

			GLuint _vao = (GLuint) 0;
			GLuint _vbo = (GLuint) 0;
			GLuint _ebo = (GLuint) 0;

			unsigned int _num_indices = 0;
			unsigned int _num_vertices = 0;
		};
	}
}

#endif