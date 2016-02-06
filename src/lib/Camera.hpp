#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.hpp"

class Camera {
public:
	Camera(Transform* transform, glm::mat4 projection){
		this->mTransform = transform;
		this->mProjection = projection;
		this->mUp = glm::vec3(0.0f, 1.0f, 0.0f);
		this->mForward = glm::vec3(0.0f, 0.0f, 1.0f);
	}

	glm::mat4 GetViewProjection() const {
		return mProjection * glm::lookAt(*mTransform->GetPosition(),
										 mForward,
										 mUp);
	}
	Transform* mTransform;
	
private:
	glm::mat4 mProjection;
	glm::vec3 mUp;
	glm::vec3 mForward;
};

#endif
