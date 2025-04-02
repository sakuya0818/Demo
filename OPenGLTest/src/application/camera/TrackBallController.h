#pragma once

#include "../../glframework/core.h"
#include "CameraControl.h"

class TrackBallController : public CameraControl
{
public:
	TrackBallController();
	~TrackBallController();

	void onCursor(double xPos, double yPos) override;

	void onScroll(double offset) override;

private:
	void pitch(float angle);

	void yaw(float angle);

private:
	float mMoveSpeed = 0.005f;
};