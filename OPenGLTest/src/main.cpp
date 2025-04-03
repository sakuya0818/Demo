#include <iostream>

#include "glframework/core.h"
#include "glframework/shader.h"
#include "glframework/texture.h"
#include "application/Application.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/TrackBallController.h"
#include "glframework/Geometry.h"

GLuint vao;
Shader* shader = nullptr;
Texture* texture = nullptr;
Texture* texture1 = nullptr;
glm::mat4 transform(1.0);
glm::mat4 viewMatrix(1.0);
glm::mat4 orthoMatrix(1.0);
glm::mat4 perspectiveMatrix(1.0);
Geometry* geometry = nullptr;

PerspectiveCamera* camera = nullptr;
TrackBallController* cameraControl = nullptr;

void OnResize(int width, int height)
{
	std::cout << "窗体大小:" << width << height << std::endl;
	glViewport(0, 0, width, height);
}

void OnKey(int key, int action, int mods)
{
	cameraControl->onKey(key, action, mods);
}

void OnMouse(int button, int action, int mods)
{
	double x, y;
	Application::getInstance()->getCursorPosition(&x, &y);
	cameraControl->onMouse(button, action, x, y);
}

void OnCursor(double xPos, double yPos)
{
	cameraControl->onCursor(xPos, yPos);
}

void OnScroll(double offset)
{
	cameraControl->onScroll(offset);
}

// 旋转变化
void doRotationTransform()
{
	transform = glm::rotate(glm::identity<glm::mat4>(), glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
}

// 平移变化
void doTranslationTransform()
{
	transform = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.5f, 0.0f, 0.0f));
}

// 缩放变换
void doScaleTransform()
{
	transform = glm::scale(glm::identity<glm::mat4>(), glm::vec3(0.5f, 0.5f, 1.0f));
}

// 旋转和平移变换结合
void doTransform()
{
	glm::mat4 rotateMat = glm::rotate(glm::identity<glm::mat4>(), glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 translateMat = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.5f, 0.0f, 0.0f));
	transform = translateMat * rotateMat;
}

// 准备SingleBuffer数据
void prepareSingleBuffer()
{
	// 创建盒体
	geometry = Geometry::createBox(1.0f);

	// 创建球体
	//geometry = Geometry::createSphere(1.0f);
}

// 准备Shader
void prepareShader()
{
	shader = new Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
}

// 准备Texture
void prepareTexture()
{
	texture = new Texture("assets/textures/pikaqiu.jpg", 0);
	texture1 = new Texture("assets/textures/111.jpg", 1);
}

// 准备相机
void prepareCamera()
{
	camera = new PerspectiveCamera(60.0f, (float)Application::getInstance()->getWidth() / (float)Application::getInstance()->getHeight(), 0.1f, 1000.0f);
	cameraControl = new TrackBallController();
	cameraControl->setCamera(camera);

	//viewMatrix = glm::lookAt(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//std::cout << "111111111111111:" << glm::to_string(viewMatrix) << std::endl;
}

void prepareOrtho()
{
	// 正交投影矩阵
	// left:左边界，right:右边界，bottom:底边界，top:顶边界，near:近裁剪面，far:远裁剪面
	orthoMatrix = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 100.0f);
	std::cout << "222222222222222:" << glm::to_string(orthoMatrix) << std::endl;
}

void preparePerspective()
{
	// 透视投影矩阵
	// fovy:y轴的视野角度，aspect:宽高比，near:近裁剪面，far:远裁剪面
	perspectiveMatrix = glm::perspective(glm::radians(60.0f), (float)Application::getInstance()->getWidth() / (float)Application::getInstance()->getHeight(), 0.1f, 100.0f);
	std::cout << "333333333333333:" << glm::to_string(perspectiveMatrix) << std::endl;
}

// 渲染
void render()
{
	// 画布清理
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 绑定当前的program
	shader->beigin();

	shader->setInt("sampler", 0);
	shader->setInt("sampler1", 1);

	shader->setMatrix4x4("transform", transform);
	shader->setMatrix4x4("view", camera->getViewMatrix());
	shader->setMatrix4x4("projection", camera->getProjectionMatrix());

	// 绑定当前的vao
	glBindVertexArray(geometry->getVao());

	// 绘制三角形
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shader->end();
}

int main()
{
	// 初始化窗体
	if (!Application::getInstance()->init(800, 600))
	{
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// 设置监听窗口大小变化和按键回调
	Application::getInstance()->setResizeCallback(OnResize);
	Application::getInstance()->setKeyBoardCallback(OnKey);
	Application::getInstance()->setMouseCallback(OnMouse);
	Application::getInstance()->setCursorCallback(OnCursor);
	Application::getInstance()->setScrollCallback(OnScroll);

	// 准备Shader和vao，vbo
	prepareShader();
	prepareSingleBuffer();
	prepareTexture();
	prepareCamera();

	// 执行窗体循环
	while (Application::getInstance()->update())
	{
		cameraControl->update();
		render();
	}

	// 释放资源
	Application::getInstance()->destory();
	delete texture;
	
	return 0;
}