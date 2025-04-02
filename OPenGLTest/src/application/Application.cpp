#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Application* Application::mInstance = nullptr;
Application::Application()
{
}

Application::~Application()
{
}

Application* Application::getInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new Application();
	}

	return mInstance;
}

bool Application::init(const int& width, const int& height)
{
	mWidth = width;
	mHeight = height;

	// glfw初始化并设置版本
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗体，并为当前窗体创建OPenGL上下文
	mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGLTest", nullptr, nullptr);
	if (mWindow == nullptr)
	{
		return false;
	}
	// 指定的窗口的 OpenGL 上下文设置为当前线程的当前上下文
	glfwMakeContextCurrent(mWindow);

	// 使用glad加载所有当前版本的OPenGL函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Could not load OpenGL functions" << std::endl;
		return false;
	}

	// 设置监听窗口大小变化和按键回调
	glfwSetFramebufferSizeCallback(mWindow, frameBufferSizeCallback);
	glfwSetKeyCallback(mWindow, keyCallback);
	glfwSetMouseButtonCallback(mWindow, mouseCallback);
	glfwSetCursorPosCallback(mWindow, cursorCallback);
	glfwSetScrollCallback(mWindow, scrollCallback);

	glfwSetWindowUserPointer(mWindow, this);

	return true;
}

bool Application::update()
{
	if (glfwWindowShouldClose(mWindow))
	{
		return false;
	}

	// 事件循环
	glfwPollEvents();

	// 切换双缓存
	glfwSwapBuffers(mWindow);

	return true;
}

void Application::destory()
{
	glfwTerminate();
}

void Application::getCursorPosition(double* x, double* y)
{
	glfwGetCursorPos(mWindow, x, y);
}

void Application::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mResizeCallback != nullptr)
	{
		self->mResizeCallback(width, height);
	}
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mKeyBoardCallback != nullptr)
	{
		self->mKeyBoardCallback(key, action, mods);
	}
}

void Application::mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mMouseCallback != nullptr)
	{
		self->mMouseCallback(button, action, mods);
	}
}

void Application::cursorCallback(GLFWwindow* window, double xPos, double yPos)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mCursorCallback != nullptr)
	{
		self->mCursorCallback(xPos, yPos);
	}
}

void Application::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mScrollCallback != nullptr)
	{
		self->mScrollCallback(yOffset);
	}
}
