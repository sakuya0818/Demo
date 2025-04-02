#include "GameCameraControl.h"

GameCameraControl::GameCameraControl()
{
}

GameCameraControl::~GameCameraControl()
{
}

void GameCameraControl::onCursor(double xPos, double yPos)
{
	float deltaX = (xPos - mCurrentX) * mSensitivity;
	float deltaY = (yPos - mCurrentY) * mSensitivity;

	if (mRightMouseDown)
	{
		pitch(deltaY);
		yaw(deltaX);
	}

	mCurrentX = xPos;
	mCurrentY = yPos;
}

void GameCameraControl::pitch(float angle)
{
	mPitch += angle;
	if (mPitch > 89.0f || mPitch < -89.0f)
	{
		mPitch -= angle;
		return;
	}

	auto mat = glm::rotate(glm::identity<glm::mat4>(), glm::radians(angle), mCamera->mRight);
	mCamera->mUp = glm::vec3(mat * glm::vec4(mCamera->mUp, 0.0f));
}

void GameCameraControl::yaw(float angle)
{
	auto mat = glm::rotate(glm::identity<glm::mat4>(), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	mCamera->mUp = glm::vec3(mat * glm::vec4(mCamera->mUp, 0.0f));
	mCamera->mRight = glm::vec3(mat * glm::vec4(mCamera->mRight, 0.0f));
}
