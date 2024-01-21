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

#include <glm/vec3.hpp>
#include <memory>
#include <string>

class Texture;

struct Material
{
public:
	Material(){}
	
	void LoadDiffuseTex(const std::string& iPath);
	void LoadNormalTex(const std::string& iPath);
	void LoadSpecularExponentTex(const std::string& iPath);

	Texture* GetDiffuseTex() const;
	Texture* GetNormalTex() const;
	Texture* GetSpecularExponentTex() const;
	
	void SetAmbientColor(const glm::vec3 iAmbientColor) 
	{
		mAmbientColor = iAmbientColor;
	}

	const glm::vec3& GetAmbientColor() const 
	{ 
		return mAmbientColor; 
	}

	void SetDiffuseColor(const glm::vec3 iDiffuseColor)
	{
		mDiffuseColor = iDiffuseColor;
	}

	const glm::vec3& GetDiffuseColor() const
	{
		return mDiffuseColor;
	}

	void SetSpecularColor(const glm::vec3 iSpecularColor)
	{
		mSpecularColor = iSpecularColor;
	}

	const glm::vec3& GetSpecularColor() const
	{
		return mSpecularColor;
	}

	float GetSpecularExponent() const { return mSpecularExponent; }

private:
	Texture* mDiffuseTex = nullptr;
	Texture* mNormalTex = nullptr;
	Texture* mSpecularExponentTex = nullptr;

	glm::vec3 mAmbientColor{0.f, 0.f, 0.f};
	glm::vec3 mDiffuseColor{0.f, 0.f, 0.f};
	glm::vec3 mSpecularColor{0.f, 0.f, 0.f};
	float mSpecularExponent = 16.f;
};