#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	// 创建Shader程序
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

	// 为Shader程序输入Shader代码
	glShaderSource(vertex, 1, &vertexShaderSource, nullptr);
	glShaderSource(fragment, 1, &fragmentShaderSource, nullptr);

	// 编译Shader
	glCompileShader(vertex);
	// 检查是否编译成功
	checkShaderErrors(mProgram, "COMPILE");

	glCompileShader(fragment);
	checkShaderErrors(mProgram, "COMPILE");

	// 创建program
	mProgram = glCreateProgram();

	// 连接program和Shader
	glAttachShader(mProgram, vertex);
	glAttachShader(mProgram, fragment);

	// 链接program
	glLinkProgram(mProgram);
	checkShaderErrors(mProgram, "LINK");

	// 清理
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
}

void Shader::beigin()
{
	glUseProgram(mProgram);
}

void Shader::end()
{
	glUseProgram(0);
}

void Shader::setFloat(const std::string& name, float value)
{
	GLuint location = glGetUniformLocation(mProgram, name.c_str());
	glUniform1f(location, value);
}

void Shader::setVector3(const std::string& name, float x, float y, float z)
{
	GLuint location = glGetUniformLocation(mProgram, name.c_str());
	glUniform3f(location, x, y, z);
}

void Shader::setVector3(const std::string& name, float* values)
{
	GLuint location = glGetUniformLocation(mProgram, name.c_str());
	glUniform3fv(location, 1, values);
}

void Shader::setInt(const std::string& name, int value)
{
	GLuint location = glGetUniformLocation(mProgram, name.c_str());
	glUniform1i(location, value);
}

void Shader::setMatrix4x4(const std::string& name, glm::mat4 value)
{
	GLuint location = glGetUniformLocation(mProgram, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::checkShaderErrors(GLuint shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type == "COMPILE")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			std::cout << "Error::Shader_Compilation_Error of type:" << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else if (type == "LINK")
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			std::cout << "Error::Program_Link_Error of type:" << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		std::cout << "Error::Unknown type of shader error check" << std::endl;
	}
}
