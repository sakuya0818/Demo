#pragma once

#include "../../glframework/core.h"
#include "CameraControl.h"

class GameCameraControl : public CameraControl
{
public:
	GameCameraControl();
	~GameCameraControl();

	void onCursor(double xPos, double yPos) override;
private:
};