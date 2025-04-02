#pragma once

#include "../../glframework/core.h"
#include "CameraControl.h"

class GameCameraControl : public CameraControl
{
public:
	GameCameraControl();
	~GameCameraControl();

	void onCursor(double xPos, double yPos) override;

	void update() override;

	void setSpeed(float speed) { mSpeed = speed; }

private:
	void pitch(float angle);

	void yaw(float angle);

private:
	float mPitch = 0.0f;
	float mSpeed = 0.1f;
};