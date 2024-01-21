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

#include "Frustum.h"

class OrthographicFrustum : public Frustum
{

public:
	OrthographicFrustum();

	glm::mat4 ProjectionMatrix() override;

	void SetLeft(float iLeft) { mLeft = iLeft; }
	void SetRight(float iRight) { mRight = iRight; }
	void SetBottom(float iBottom) { mBottom = iBottom; }
	void SetTop(float iTop) { mTop = iTop; }
	void SetNear(float iNear) { mNear = iNear; }
	void SetFar(float iFar) { mFar = iFar; }

	float GetLeft() const { return mLeft; }
	float GetRight() const { return mRight; }
	float GetBottom() const { return mBottom; }
	float GetTop() const { return mTop; }
	float GetNearPlane() const { return mNear; }
	float GetFarPlane() const { return mFar; }

private:
	float mLeft = -20.f;
	float mRight = 20.0f;
	float mBottom = -20.0f;
	float mTop = 20.0f;
	float mNear = 0.1f;
	float mFar = 1000.0f;
};