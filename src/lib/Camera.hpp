#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include "Transform.hpp"

class Camera {
public:
	Camera(Transform* transform, glm::fmat4 projection){
		this->mTransform = transform;
		this->mProjection = projection;
		this->mUp = glm::fvec3(0.0f, 1.0f, 0.0f);
		this->mDirection = glm::fvec3(0.0f, 0.0f, 1.0f);
	}

	inline void SetAspectRatio(float fov, float aspectRatio) {
		this->mProjection = glm::perspective(
			fov, aspectRatio, 0.1f, 100.0f);
	}

	glm::mat4 GetViewProjection() const {
		return mProjection * glm::lookAt(*mTransform->GetPosition(),
										 *mTransform->GetPosition() + GetDirection(),
										 GetUp());
	}

	inline const glm::fvec3 GetDirection() const {
		return glm::fvec3(
			cos(mTransform->GetRotation()->y) * sin(mTransform->GetRotation()->x),
			sin(mTransform->GetRotation()->y),
			cos(mTransform->GetRotation()->y) * cos(mTransform->GetRotation()->x));
	}

	inline const glm::fvec3 GetRight() const {
		return glm::fvec3(
			sin(mTransform->GetRotation()->x - 3.14f / 2.0f),
			0,
			cos(mTransform->GetRotation()->x - 3.14f / 2.0f)
			);
	}

	inline const glm::fvec3 GetUp() const {
		return glm::cross(GetRight(), GetDirection());
	}

	Transform* mTransform;

private:
	glm::fmat4 mProjection;
	glm::fvec3 mUp;
	glm::fvec3 mDirection;

};

#endif
