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

#include <spdlog/spdlog.h>
#include <fstream>
#include <sstream>
#include <chrono>

#include "Shader.h"

using namespace std::chrono;

Shader::Shader(const std::string& iPath, EShaderStage iShaderStage)
	:mShaderStage(iShaderStage)
{
	auto wClockStart = high_resolution_clock::now();

	mFilename = iPath.substr(iPath.find_last_of("/\\") + 1);

	std::string wShaderCode;
	std::ifstream wShaderFile;

	// ensure ifstream objects can throw exceptions:
	wShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		wShaderFile.open(iPath);
		std::stringstream vShaderStream;
		// read file's buffer contents into streams
		vShaderStream << wShaderFile.rdbuf();
		// close file handlers
		wShaderFile.close();
		// convert stream into string
		wShaderCode = vShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		spdlog::critical("Shader file {0:s} not Successfully Read !", iPath);
	}

	const char* wShaderCodeStr = wShaderCode.c_str();
	
	int success;
	char infoLog[512];
	GLuint wShaderStage;
	std::string wShaderStageStr;

	GetShaderType(iShaderStage, wShaderStage, wShaderStageStr);

	mShaderHandle = glCreateShader(wShaderStage);
	glShaderSource(mShaderHandle, 1, &wShaderCodeStr, NULL);
	glCompileShader(mShaderHandle);
	// print compile errors if any
	glGetShaderiv(mShaderHandle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(mShaderHandle, 512, NULL, infoLog);
		spdlog::critical("Failed to compile {0:s}. ({1:s} shader)\n{2:s}", mFilename, wShaderStageStr, infoLog);
	};

	auto wClockStop = high_resolution_clock::now();
	auto wDuration = duration_cast<milliseconds>(wClockStop - wClockStart);
	
	spdlog::info("{0:s} shader parsed and compiled in {1:d} ms !", mFilename, wDuration.count());
}

void Shader::GetShaderType(EShaderStage iShaderStage, GLuint& oOpenglStage, std::string& oStageString)
{
	switch (iShaderStage)
	{
	case EShaderStage::eVertex:
		oOpenglStage = GL_VERTEX_SHADER;
		oStageString = "Vertex";
		break;
	case EShaderStage::eFragment:
		oOpenglStage = GL_FRAGMENT_SHADER;
		oStageString = "Fragment";
		break;
	case EShaderStage::eGeometry:
		oOpenglStage = GL_GEOMETRY_SHADER;
		oStageString = "Geometry";
		break;
	default:
		oOpenglStage = 0;
		oStageString = "Unspecified";
	}
}
