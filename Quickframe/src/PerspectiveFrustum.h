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

class PerspectiveFrustum : public Frustum
{

public:
	PerspectiveFrustum();

	glm::mat4 ProjectionMatrix() override;

	void SetFOV(float iFov) { mFov = iFov; }
	void SetAspectRatio(float iAR) { mAspectRatio = iAR; }
	void SetNearPlane(float iNear) { mNear = iNear; }
	void SetFarPlane(float iFar) { mFar = iFar; }

	float GetFOV() const { return mFov; }
	float GetAspectRatio() const { return mAspectRatio; }
	float GetNearPlane() const { return mNear; }
	float GetFarPlane() const { return mFar; }

private:
	float mFov = 45.f; // In Degrees
	float mAspectRatio = 1.f;
	float mNear = 0.1f;
	float mFar = 1000.f;

};