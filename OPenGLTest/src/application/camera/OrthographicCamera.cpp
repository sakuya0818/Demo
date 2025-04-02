#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(float l, float r, float t, float b, float n, float f)
{
	mLeft = l;
	mRight = r;
	mTop = t;
	mBottom = b;
	mNear = n;
	mFar = f;
}

OrthographicCamera::~OrthographicCamera()
{

}

glm::mat4 OrthographicCamera::getProjectionMatrix()
{
	float scale = std::pow(2.0f, mScale);
	return glm::ortho(mLeft * scale, mRight * scale, mTop * scale, mBottom * scale, mNear, mFar);
}

void OrthographicCamera::scale(float deltaScale)
{
	mScale += deltaScale;
}
