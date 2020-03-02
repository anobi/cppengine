#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>

#include "../scene.hpp"
#include "../shader.hpp"
#include "render_entity.hpp"
#include "r_light.hpp"

namespace Rendering {
	class Renderer {
	public:
		bool Init() { return false; };
		void Render(std::shared_ptr<Camera> camera, std::shared_ptr<Shader> shader) {};
		void Shutdown() {};

		void UpdateTick(const int tick) { this->m_tick = tick; }
		int GetTick() { return this->m_tick; }

		void UpdateResolution(const int w, const int h) { this->resolution = glm::fvec2(w, h); }
		glm::fvec2 GetResolution() { return this->resolution; }

	protected:

		int m_tick = 0;
		glm::fvec2 resolution;

		std::vector<std::shared_ptr<Rendering::RenderEntity>> _renderEntities;
		std::vector<std::shared_ptr<Rendering::Light>> _staticLights;
		std::vector<std::shared_ptr<Rendering::Light>> _dynamicLights;
		std::vector<std::shared_ptr<Shader>> _shaders;
	};
}

#endif
