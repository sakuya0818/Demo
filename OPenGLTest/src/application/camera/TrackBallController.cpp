#include "TrackBallController.h"

#include <iostream>

TrackBallController::TrackBallController()
{
}

TrackBallController::~TrackBallController()
{
}

void TrackBallController::onCursor(double xPos, double yPos)
{
	if (mLeftMouseDown)
	{
		// 调整相机的各类参数
		// 计算经线和纬线旋转的增量角度（正负都有可能）
		float deltaX = (xPos - mCurrentX) * mSensitivity;
		float deltaY = (yPos - mCurrentY)* mSensitivity;

		// 分开pitch和yaw各自计算
		pitch(-deltaY);
		yaw(-deltaX);
	}
	else if (mRightMouseDown)
	{
		// 平移
		float deltaX = (xPos - mCurrentX) * mMoveSpeed;
		float deltaY = (yPos - mCurrentY) * mMoveSpeed;
		mCamera->mPosition += mCamera->mRight * deltaX;
		mCamera->mPosition += mCamera->mUp * deltaY;
	}

	mCurrentX = xPos;
	mCurrentY = yPos;
}

void TrackBallController::onScroll(double offset)
{
	mCamera->scale(mScaleSpeed * offset);
}

void TrackBallController::pitch(float angle)
{
	// pitch旋转是绕着相机本身的right向量在旋转，相机的up向量也会随之变化
	// 先计算这个旋转的矩阵，然后把这个矩阵应用到相机的up向量上
	auto mat = glm::rotate(glm::identity<glm::mat4>(), glm::radians(angle), mCamera->mRight);
	mCamera->mUp = glm::vec3(mat * glm::vec4(mCamera->mUp, 0.0f));
	mCamera->mPosition = glm::vec3(mat * glm::vec4(mCamera->mPosition, 1.0f));
}

void TrackBallController::yaw(float angle)
{
	// yaw旋转是绕着世界坐标系的up向量在旋转，相机的up和right向量也会随之变化
	// 先计算这个旋转的矩阵，然后把这个矩阵应用到相机的up和right向量上
	auto mat = glm::rotate(glm::identity<glm::mat4>(), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	mCamera->mUp = glm::vec3(mat * glm::vec4(mCamera->mUp, 0.0f));
	mCamera->mRight = glm::vec3(mat * glm::vec4(mCamera->mRight, 0.0f));
	mCamera->mPosition = glm::vec3(mat * glm::vec4(mCamera->mPosition, 1.0f));
}
