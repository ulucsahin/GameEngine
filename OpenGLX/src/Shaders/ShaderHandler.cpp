#include "ShaderHandler.h"
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ErrorHandling/ErrorHandler.h"


ShaderHandler::ShaderHandler()
{
	// Create Program
	programID = glCreateProgram();
}

ShaderHandler::~ShaderHandler()
{
}

ShaderInfo ShaderHandler::LoadShader(const char* shaderPath, int shaderType)
{
	/* shaderType: GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
	*/
    GLuint ShaderID = glCreateShader(shaderType);
   
    std::string ShaderCode;
    std::ifstream ShaderStream(shaderPath, std::ios::in);
	if (ShaderStream.is_open()) 
	{
		std::stringstream sstr;
		sstr << ShaderStream.rdbuf();
		ShaderCode = sstr.str();
		ShaderStream.close();
		shaderIDs.push_back(ShaderID);
		return { ShaderID, ShaderCode };
	}
	else {
		printf("Couldn't open %s \n", shaderPath);
		getchar();
		return {0, std::string()};
	}

	return { 0, std::string() };
}

void ShaderHandler::CompileAttachShader(ShaderInfo& shaderInfo)
{
	char const* shaderSourcePointer = shaderInfo.code.c_str();
	glShaderSource(shaderInfo.ID, 1, &shaderSourcePointer, NULL);
	glCompileShader(shaderInfo.ID);
	glAttachShader(programID, shaderInfo.ID);
}

int ShaderHandler::GetUniformLocation(const std::string& name)
{
	int location = glGetUniformLocation(programID, name.c_str());

	if (location == -1)
	{
		std::cout << "Error: uniform " << name << " doesn't exist!\n";
		exit(-1);
	}	

	return location;
}
