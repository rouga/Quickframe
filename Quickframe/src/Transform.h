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
#include <glm/mat4x4.hpp>

class Transform
{
public:
	Transform();
	Transform(glm::vec3 iPos, glm::vec3 iRot, glm::vec3 iScale);

	void Translate(glm::vec3 iTranslation);
	void SetWorldPos(glm::vec3 iWorldPos);
	void Rotate(glm::vec3 iRotation);
	void SetRotation(glm::vec3 iRotation);
	void Scale(glm::vec3 iScale);
	void SetScale(glm::vec3 iScale);

	const glm::mat4 GetWorldMatrix();
	const glm::vec3& GetPos() const { return mPos; }
	const glm::vec3& GetScale() const { return mScale; }
private:

	glm::vec3 mScale{ 1.f, 1.f, 1.f };
	glm::vec3 mRotation{0.f, 0.f, 0.f};
	glm::vec3 mPos{0.f, 0.f, 0.f};
};