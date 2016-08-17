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
	Camera() {
		mTransform = Transform();
		this->mProjection = glm::mat4();
		this->mUp = glm::fvec3(0.0f, 1.0f, 0.0f);
		this->mDirection = glm::fvec3(0.0f, 0.0f, 1.0f);
	}

	Camera(glm::fmat4 projection){
		mTransform = Transform();
		this->mProjection = projection;
		this->mUp = glm::fvec3(0.0f, 1.0f, 0.0f);
		this->mDirection = glm::fvec3(0.0f, 0.0f, 1.0f);
	}

	inline void SetAspectRatio(float fov, float aspectRatio) {
		this->mProjection = glm::perspective(
			fov, aspectRatio, 0.1f, 100.0f);
	}

	inline glm::fmat4 GetView() {
		return glm::lookAt(mTransform.GetPosition(),
						   mTransform.GetPosition() + GetDirection(),
						   GetUp());
	}

	inline glm::fmat4 GetProjection(){
		return mProjection;
	}

	inline glm::fmat4 GetViewProjection() {
		return mProjection * GetView();
	}

	inline glm::fvec3 GetPosition() { return mTransform.GetPosition(); }

	inline glm::fvec3 GetDirection() {
		glm::fvec3 rotation = mTransform.GetRotation();
		return glm::fvec3(
			cos(rotation.y) * sin(rotation.x),
			sin(rotation.y),
			cos(rotation.y) * cos(rotation.x));
	}

	inline glm::fvec3 GetRight() {
		glm::fvec3 rotation = mTransform.GetRotation();
		return glm::fvec3(
			sin(rotation.x - 3.14f / 2.0f),
			0,
			cos(rotation.x - 3.14f / 2.0f));
	}

	inline glm::fvec3 GetUp() {
		return glm::cross(GetRight(), GetDirection());
	}

	Transform mTransform;

private:
	glm::fmat4 mProjection;
	glm::fvec3 mUp;
	glm::fvec3 mDirection;

};

#endif
