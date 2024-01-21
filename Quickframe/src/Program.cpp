/*
MIT License

Copyright (c) 2022 Amin O-M

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <chrono>
#include <spdlog/spdlog.h>

#include "Program.h"

using namespace std::chrono;

Program::Program(const std::vector<Shader>& iShaders)
{
	auto wClockStart = high_resolution_clock::now();
	mProgramHandle = glCreateProgram();

	PipelineShaders wPipeline{};

	for (const Shader& wShader : iShaders)
	{
		glAttachShader(mProgramHandle, wShader.GetShaderHandle());

		switch (wShader.GetStage())
		{
		case Shader::EShaderStage::eVertex:
			wPipeline.VS = wShader;
			break;
		case Shader::EShaderStage::eGeometry:
			wPipeline.GS = wShader;
			break;
		case Shader::EShaderStage::eFragment:
			wPipeline.FS = wShader;
			break;
		}
	}

	// Print Infos 
	spdlog::info("Program {0:d} Shaders :", mProgramHandle);
	spdlog::info("\tProgram {0:d} VS : {1:s}", mProgramHandle, wPipeline.VS.GetFilename());
	spdlog::info("\tProgram {0:d} GS : {1:s}", mProgramHandle, wPipeline.GS.GetFilename());
	spdlog::info("\tProgram {0:d} FS : {1:s}", mProgramHandle, wPipeline.FS.GetFilename());

	int wSuccess = 0;
	char wErrorLog[1024] = { 0 };

	// Link the program
	glLinkProgram(mProgramHandle);
	glGetProgramiv(mProgramHandle, GL_LINK_STATUS, &wSuccess);
	if (wSuccess == 0) 
	{
		glGetProgramInfoLog(mProgramHandle, sizeof(wErrorLog), NULL, wErrorLog);
		spdlog::critical("Error Linking Program : {0:s}", wErrorLog);
	}

	// Validate the program
	glValidateProgram(mProgramHandle);
	glGetProgramiv(mProgramHandle, GL_VALIDATE_STATUS, &wSuccess);
	if (!wSuccess)
	{
		glGetProgramInfoLog(mProgramHandle, sizeof(wErrorLog), NULL, wErrorLog);
		spdlog::critical("Error Validating Program : {0:s}", wErrorLog);
	}

	auto wClockStop = high_resolution_clock::now();
	auto wDuration = duration_cast<milliseconds>(wClockStop - wClockStart);
	spdlog::info("Program {0:d} compiled in {1:d} ms !", mProgramHandle, wDuration.count());
}

void Program::Bind()
{
	glUseProgram(mProgramHandle);
}

void Program::Unbind()
{
	glUseProgram(0);
}

void Program::SetUniform1i(const std::string& iName, int iValue)
{
	GLuint wLoc = GetUniformLocation(iName);
	if (wLoc != 0xFFFFFFFF)
	{
		glUniform1i(wLoc, iValue);
		return;
	}

	mUniformsMap[iName] = glGetUniformLocation(mProgramHandle, iName.c_str());
	glUniform1i(glGetUniformLocation(mProgramHandle, iName.c_str()), iValue);
}

void Program::SetUniform1f(const std::string& iName, float iValue)
{
	GLuint wLoc = GetUniformLocation(iName);
	if (wLoc != 0xFFFFFFFF)
	{
		glUniform1f(wLoc, iValue);
		return;
	}

	mUniformsMap[iName] = glGetUniformLocation(mProgramHandle, iName.c_str());
	glUniform1f(glGetUniformLocation(mProgramHandle, iName.c_str()), iValue);
}

void Program::SetUniform3f(const std::string& iName, glm::vec3 iValue)
{
	GLuint wLoc = GetUniformLocation(iName);
	if (wLoc != 0xFFFFFFFF)
	{
		glUniform3fv(wLoc, 1, glm::value_ptr(iValue));
		return;
	}

	mUniformsMap[iName] = glGetUniformLocation(mProgramHandle, iName.c_str());
	glUniform3fv(glGetUniformLocation(mProgramHandle, iName.c_str()), 1, glm::value_ptr(iValue));
}

void Program::SetUniform4f(const std::string& iName, glm::vec4 iValue)
{
	GLuint wLoc = GetUniformLocation(iName);
	if (wLoc != 0xFFFFFFFF)
	{
		glUniform4fv(wLoc, 1, glm::value_ptr(iValue));
		return;
	}
	
	mUniformsMap[iName] = glGetUniformLocation(mProgramHandle, iName.c_str());
	glUniform4fv(glGetUniformLocation(mProgramHandle, iName.c_str()), 1, glm::value_ptr(iValue));
}

void Program::SetUniform4f(const std::string& iName, float iX, float iY, float iZ, float iW)
{
	GLuint wLoc = GetUniformLocation(iName);
	if (wLoc != 0xFFFFFFFF)
	{
		glUniform4f(wLoc, iX, iY, iZ, iW);
		return;
	}

	mUniformsMap[iName] = glGetUniformLocation(mProgramHandle, iName.c_str());
	glUniform4f(glGetUniformLocation(mProgramHandle, iName.c_str()), iX, iY, iZ, iW);
}

void Program::SetUniformMatrix3f(const std::string& iName, const glm::mat3& iMatrix)
{
	GLuint wLoc = GetUniformLocation(iName);
	if (wLoc != 0xFFFFFFFF)
	{
		glUniformMatrix3fv(wLoc, 1, GL_FALSE, glm::value_ptr(iMatrix));
		return;
	}

	mUniformsMap[iName] = glGetUniformLocation(mProgramHandle, iName.c_str());
	glUniformMatrix3fv(glGetUniformLocation(mProgramHandle, iName.c_str()), 1, GL_FALSE, glm::value_ptr(iMatrix));
}

void Program::SetUniformMatrix4f(const std::string& iName, const glm::mat4& iMatrix)
{
	GLuint wLoc = GetUniformLocation(iName);
	if (wLoc != 0xFFFFFFFF)
	{
		glUniformMatrix4fv(wLoc, 1, GL_FALSE, glm::value_ptr(iMatrix));
		return;
	}

	mUniformsMap[iName] = glGetUniformLocation(mProgramHandle, iName.c_str());
	glUniformMatrix4fv(glGetUniformLocation(mProgramHandle, iName.c_str()), 1, GL_FALSE, glm::value_ptr(iMatrix));
}

GLuint Program::GetUniformLocation(const std::string& iName)
{
	auto it = mUniformsMap.find(iName);
	if (it != mUniformsMap.end())
	{
		return (*it).second;
	}

	return 0xFFFFFFFF;
}
