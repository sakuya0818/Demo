#include <iostream>

#include "glframework/core.h"
#include "glframework/shader.h"
#include "glframework/texture.h"
#include "application/Application.h"

GLuint vao;
Shader* shader = nullptr;
Texture* texture = nullptr;
Texture* texture1 = nullptr;
glm::mat4 transform(1.0);
glm::mat4 viewMatrix(1.0);

void OnResize(int width, int height)
{
	std::cout << "窗体大小:" << width << height << std::endl;
	glViewport(0, 0, width, height);
}

void OnKeyBoard(int key, int action, int mods)
{
	if (key == GLFW_KEY_W)
	{
		std::cout << "按下：" << key << std::endl;
	}
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
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};
	float colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 1, 3
	};

	// 生成vbo并绑定
	GLuint posVbo, colorVbo, uvVbo;
	glGenBuffers(1, &posVbo);
	glGenBuffers(1, &colorVbo);
	glGenBuffers(1, &uvVbo);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	// 生成ebo并绑定
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 生成vao并绑定
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 把描述信息加入vbo
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void *)0);

	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindVertexArray(0);
}

// 准备InterleavedBuffer数据
void prepareInterleavedBuffer()
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	// 创建唯一的vbo
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 创建vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 为vbo绑定数据
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void *)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));

	// 结束vao绑定
	glBindVertexArray(0);
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
	viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

// 渲染
void render()
{
	// 画布清理
	glClear(GL_COLOR_BUFFER_BIT);

	// 绑定当前的program
	shader->beigin();

	shader->setInt("sampler", 0);
	shader->setInt("sampler1", 1);

	shader->setMatrix4x4("transform", transform);
	shader->setMatrix4x4("view", viewMatrix);

	// 绑定当前的vao
	glBindVertexArray(vao);

	// 绘制三角形
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

	// 设置监听窗口大小变化和按键回调
	Application::getInstance()->setResizeCallback(OnResize);
	Application::getInstance()->setKeyBoardCallback(OnKeyBoard);

	// 准备Shader和vao，vbo
	prepareShader();
	prepareSingleBuffer();
	prepareTexture();
	prepareCamera();

	// 执行窗体循环
	while (Application::getInstance()->update())
	{
		render();
	}

	// 释放资源
	Application::getInstance()->destory();
	delete texture;
	
	return 0;
}