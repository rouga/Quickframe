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

#include <vector>
#include <unordered_map>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

/**
   @brief : A program Object contains all shader stages
					  defining the programmable GPU pipeline
*/
class Program
{
	struct PipelineShaders {
		Shader VS;
		Shader GS;
		Shader FS;
	};

public:

	Program(const std::vector<Shader>& iShaders);
	void Bind();
	void Unbind();
	void SetUniform1i(const std::string& iName, int iValue);
	void SetUniform1f(const std::string& iName, float iValue);
	void SetUniform3f(const std::string& iName, glm::vec3 iValue);
	void SetUniform4f(const std::string& iName, glm::vec4 iValue);
	void SetUniform4f(const std::string& iName, float iX, float iY, float iZ, float iW);
	void SetUniformMatrix3f(const std::string& iName, const glm::mat3& iMatrix);
	void SetUniformMatrix4f(const std::string& iName, const glm::mat4& iMatrix);

private:
	GLuint GetUniformLocation(const std::string& iName);
	GLuint mProgramHandle;

	std::unordered_map<std::string, GLuint> mUniformsMap;
};