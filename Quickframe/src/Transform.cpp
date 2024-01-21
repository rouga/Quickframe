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

#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"

Transform::Transform()
{
}

Transform::Transform(glm::vec3 iPos, glm::vec3 iRot, glm::vec3 iScale)
	:mPos(iPos),
	mRotation(iRot),
	mScale(iScale)
{
}

void Transform::Translate(glm::vec3 iTranslation)
{
	mPos += iTranslation;
}

void Transform::SetWorldPos(glm::vec3 iWorldPos)
{
	mPos = iWorldPos;
}

void Transform::Rotate(glm::vec3 iRotation)
{
	mRotation += iRotation;
}

void Transform::SetRotation(glm::vec3 iRotation)
{
	mRotation = iRotation;
}

void Transform::Scale(glm::vec3 iScale)
{
	mScale *= iScale;
}

void Transform::SetScale(glm::vec3 iScale)
{
	mScale = iScale;
}

const glm::mat4 Transform::GetWorldMatrix()
{
	glm::mat4 wTranslationMat = glm::mat4(1.f);
	glm::mat4 wRotationMat = glm::mat4(1.f);
	glm::mat4 wScaleMat = glm::mat4(1.f);

	wTranslationMat = glm::translate(wTranslationMat, mPos);

	// Rotate around Y, then X, then Z
	wRotationMat = glm::rotate(wRotationMat, glm::radians(mRotation.y), glm::vec3(0.f, 1.f, 0.f));
	wRotationMat = glm::rotate(wRotationMat, glm::radians(mRotation.x), glm::vec3(1.f, 0.f, 0.f));
	wRotationMat = glm::rotate(wRotationMat, glm::radians(mRotation.z), glm::vec3(0.f, 0.f, 1.f));

	wScaleMat = glm::scale(wScaleMat, mScale);

	return (wTranslationMat * wRotationMat * wScaleMat);
}