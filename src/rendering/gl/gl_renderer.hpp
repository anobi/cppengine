#ifndef R_GL_RENDERER_H
#define R_GL_RENDERER_H

#include <vector>
#include <memory>

#include "gl_light.hpp"
#include "../renderer.hpp"

namespace GL {
	class GLRenderer : public Rendering::Renderer {
	public:
		GLRenderer() {};
		~GLRenderer() {};

		bool Init();
		void Render(std::shared_ptr<Camera> camera, std::shared_ptr<Shader> shader);
		void Shutdown();
	};
}

#endif