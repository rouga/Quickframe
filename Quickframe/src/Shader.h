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

#pragma once

#include <glad/glad.h>
#include <string>

class Shader
{

public:
	enum class EShaderStage
	{
		eVertex,
		eGeometry,
		eFragment,
		eUnspecified
	};

	Shader(){}
	Shader(const std::string& iFile, EShaderStage iShaderStage);

	const GLuint GetShaderHandle() const { return mShaderHandle; }
	const EShaderStage GetStage() const { return mShaderStage; }
	const std::string& GetFilename() const { return mFilename; }

private:
	/**
	 * @brief : Get OpenGL internal shader stage handle from a EShaderStage enum
	 * @param iShaderStage : EShaderStage as defined by Quickframe
	 * @param oOpenglStage : Output variable to contain OGL internal shader stage handle
	 * @param oStageString : Output variable containing the string defining the stage (Vertex, Geometry...)
	*/
	void GetShaderType(EShaderStage iShaderStage, GLuint& oOpenglStage, std::string& oStageString);

	std::string mFilename = "Unspecified";
	EShaderStage mShaderStage = EShaderStage::eUnspecified;
	GLuint mShaderHandle = -1;
};