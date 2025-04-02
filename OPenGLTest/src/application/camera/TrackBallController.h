#pragma once

#include "../../glframework/core.h"
#include "CameraControl.h"

class TrackBallController : public CameraControl
{
public:
	TrackBallController();
	~TrackBallController();
	void onCursor(double xPos, double yPos) override;
	void onKey(int key, int action, int mods) override;
	void onScroll(double xOffset, double yOffset) override;
	void update() override;

private:

};