#pragma once

#include "Camera.h"

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(float fovy, float aspect, float n, float f);
	~PerspectiveCamera();

	virtual glm::mat4 getProjectionMatrix() override;

	void scale(float deltaScale) override;

private:
	float mFovy = 0.0f;
	float mAspect = 0.0f;
	float mNear = 0.0f;
	float mFar = 0.0f;
};