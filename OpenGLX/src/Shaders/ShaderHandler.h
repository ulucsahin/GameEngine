#pragma once

#include <string>
#include <vector>

struct ShaderInfo
{
	unsigned int ID;
	std::string code;
};

class ShaderHandler
{
public:
	ShaderHandler();
	~ShaderHandler();

	ShaderInfo LoadShader(const char* shaderPath, int shaderType);
	void CompileAttachShader(ShaderInfo& shaderInfo);

	unsigned int GetProgramID() const { return programID; }

	int GetUniformLocation(const std::string& name);

private:
	std::vector<int> shaderIDs;
	unsigned int programID;
};

