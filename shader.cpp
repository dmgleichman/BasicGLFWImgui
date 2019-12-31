/*
 * shader.cpp
 *
 *  Created on: Dec 31, 2019
 *      Author: doug
 */


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "GL/gl3w.h"

#include "glm/glm.hpp"


#include "shader.h"

GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			error = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			error = "STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}

#define glCheckError() glCheckError_(__FILE__, __LINE__)


std::string Shader::load(std::string path)
{
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(path);
		std::stringstream shaderStream;
		// read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();
		// close file handlers
		shaderFile.close();
		// convert stream into string
		return shaderStream.str();
	} catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		return 0;
	}
}

Shader::Shader()
{
	vID = fID = gID = pID = 0;
}
Shader::~Shader()
{
}
GLuint Shader::getpID()
{
	return pID;
}
GLuint Shader::build()
{
	pID = glCreateProgram();
	glCheckError();
	glAttachShader(pID, vID);
	glCheckError();
	glAttachShader(pID, fID);
	glCheckError();
	glLinkProgram(pID);
	glCheckError();
	checkCompileErrors(pID, "PROGRAM");
	glDeleteShader(vID);
	glCheckError();
	glDeleteShader(fID);
	glCheckError();
	return pID;
}
void Shader::loadFS(std::string path)
{
	int ret;
	char infoLog[512];
	const char *code;
	mFSsource = load(path + "/shader.fs");
	code = mFSsource.c_str();
	fID = glCreateShader(GL_FRAGMENT_SHADER);
	glCheckError();
	glShaderSource(fID, 1, &code, NULL);
	glCheckError();
	glCompileShader(fID);
	glCheckError();
	checkCompileErrors(fID, "FRAGMENT");
	// check for shader compile errors
	glGetShaderiv(fID, GL_COMPILE_STATUS, &ret);
	glCheckError();
	if (!ret)
	{
		glGetShaderInfoLog(fID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}
void Shader::loadVS(std::string path)
{
	int ret;
	char infoLog[512];
	const char *code;
	mVSsource = load(path + "/shader.vs");
	code = mVSsource.c_str();
	vID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vID, 1, &code, NULL);
	glCompileShader(vID);
	checkCompileErrors(vID, "VERTEX");
	// check for shader compile errors
	glGetShaderiv(vID, GL_COMPILE_STATUS, &ret);
	if (!ret)
	{
		glGetShaderInfoLog(vID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}
void Shader::loadGS(std::string path)
{
	mGSsource = load(path + "/shader.gs");
}
void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type.compare("PROGRAM"))
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
					<< "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
					<< "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}
void Shader::use()
{
	glUseProgram(pID);
}
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(pID, name.c_str()), value);
}
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(pID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


