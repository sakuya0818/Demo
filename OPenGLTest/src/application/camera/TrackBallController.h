#pragma once

#include "../../glframework/core.h"
#include "CameraControl.h"

class TrackBallController : public CameraControl
{
public:
	TrackBallController();
	~TrackBallController();
	void onMouse(int button, int action, double xPos, double yPos);
	void onCursor(double xPos, double yPos);
	void onKey(int key, int action, int mods);
	void update();
};