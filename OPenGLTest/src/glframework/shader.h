#pragma once

#include "core.h"

#include <string>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void beigin();

	void end();

	void setFloat(const std::string& name, float value);

	void setVector3(const std::string& name, float x, float y, float z);

	void setVector3(const std::string& name, float* values);

	void setInt(const std::string& name, int value);

	void setMatrix4x4(const std::string& name, glm::mat4 value);

private:
	void checkShaderErrors(GLuint shader, std::string type);

private:
	GLuint mProgram{ 0 };
};