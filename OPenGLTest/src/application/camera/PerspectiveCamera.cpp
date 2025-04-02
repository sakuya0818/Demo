#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float n, float f)
{
	mFovy = fovy;
	mAspect = aspect;
	mNear = n;
	mFar = f;
}

PerspectiveCamera::~PerspectiveCamera()
{
}

glm::mat4 PerspectiveCamera::getProjectionMatrix()
{
	return glm::perspective(glm::radians(mFovy), mAspect, mNear, mFar);
}

void PerspectiveCamera::scale(float deltaScale)
{
	auto front = glm::cross(mUp, mRight);
	mPosition += front * deltaScale;
}
