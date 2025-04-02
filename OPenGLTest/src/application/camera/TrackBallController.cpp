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
	std::cout << "TrackBallController::onCursor" << std::endl;

	if (mLeftMouseDown)
	{
		// 调整相机的各类参数
		// 计算经线和纬线旋转的增量角度（正负都有可能）
		float deltaX = (xPos - mCurrentX) * mSensitivity;
		float deltaY = (yPos - mCurrentY)* mSensitivity;

		// 分开pitch和yaw各自计算
	}

	mCurrentX = xPos;
	mCurrentY = yPos;
}

void TrackBallController::onKey(int key, int action, int mods)
{
}

void TrackBallController::onScroll(double xOffset, double yOffset)
{
	std::cout << "TrackBallController::onScroll" << xOffset << yOffset << std::endl;
}

void TrackBallController::update()
{
}
