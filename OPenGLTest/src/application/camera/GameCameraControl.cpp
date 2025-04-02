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

void GameCameraControl::update()
{
	glm::vec3 dirction(0.0f);

	auto front = glm::cross(mCamera->mUp, mCamera->mRight);
	auto right = mCamera->mRight;

	if (mKeyMap[GLFW_KEY_W])
	{
		dirction += front;
	}
	if (mKeyMap[GLFW_KEY_S])
	{
		dirction -= front;
	}
	if (mKeyMap[GLFW_KEY_A])
	{
		dirction += right;
	}
	if (mKeyMap[GLFW_KEY_D])
	{
		dirction -= right;
	}

	// 此时dirction可能长度不为1，需要归一化
	// 也可能为0，此时不需要移动
	if (glm::length(dirction) != 0.0f)
	{
		dirction = glm::normalize(dirction);
		mCamera->mPosition += dirction * mSpeed;
	}
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
