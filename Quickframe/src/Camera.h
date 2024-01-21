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

#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>

#include "Frustum.h"
#include "View.h"

/**
 * @brief Camera Object
*/
class Camera
{
public:
	Camera(glm::vec3 iPos, float iPitch, float iYaw);

	void SetPosition(glm::vec3 iPos) 
	{ 
		mView->SetPos(iPos);
	}

	void SetDirection(glm::vec3 iDir) 
	{ 
		mView->SetDirection(iDir);
	}

	void SetFOV(float iFov);
	void SetAspectRatio(float iAR);
	void SetNearPlane(float iNear);
	void SetFarPlane(float iFar);

	void Update(double iDeltaTime);

	glm::mat4 ViewMatrix() const;
	glm::mat4 VPMatrix() const;

	const glm::vec3& WorldPos() const;

	const glm::vec3& Direction() const { return mView->GetDirection(); }

	glm::mat4 ProjectionMatrix() const { return mView->GetFrustum()->ProjectionMatrix(); }

private:
	std::unique_ptr<View> mView;

	float mSpeed = 15.0f;							// Camera Speed In meters per second
	float mMouseSensitivity = 0.1f;		//
	float mYaw = 0.0f;
	float mPitch = 0.0f;

	const glm::vec3 kWorldUp = glm::vec3{ 0.f, 1.f, 0.f };
};