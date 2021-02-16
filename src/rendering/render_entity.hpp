#ifndef R_RENDER_ENTITY
#define R_RENDER_ENTITY

#include <glm/glm.hpp>

namespace Rendering {
	class RenderEntity {
	public:
		RenderEntity() {};
		~RenderEntity() {};

		glm::fmat4 GetModelMatrix() { return this->_model_matrix; };
		void SetModelMatrix(glm::fmat4 model_matrix) { this->_model_matrix = model_matrix; }

		virtual void Draw() {};
		virtual void PostDraw() {};

	private:
		glm::fmat4 _model_matrix;
	};
}

#endif