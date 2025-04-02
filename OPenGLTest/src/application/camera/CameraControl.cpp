#include "CameraControl.h"
#include <iostream>

CameraControl::CameraControl()
{
}

CameraControl::~CameraControl()
{
}

void CameraControl::onMouse(int button, int action, double xPos, double yPos)
{
	bool pressed = action == GLFW_PRESS ? true : false;

	if (pressed)
	{
		mCurrentX = xPos;
		mCurrentY = yPos;
	}
	
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		mLeftMouseDown = pressed;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		mRightMouseDown = pressed;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		mMiddleMouseDown = pressed;
		break;
	}
}

void CameraControl::onCursor(double xPos, double yPos)
{
}

void CameraControl::onKey(int key, int action, int mods)
{
	if (action == GLFW_REPEAT)
	{
		return;
	}

	bool pressed = action == GLFW_PRESS ? true : false;

	mKeyMap[key] = pressed;
}

void CameraControl::onScroll(double offset)
{
	
}

void CameraControl::update()
{
}
