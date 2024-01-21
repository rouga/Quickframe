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
#include <spdlog/spdlog.h>

#include "Camera.h"
#include "Input.h"
#include "PerspectiveFrustum.h"

Camera::Camera(glm::vec3 iPos, float iPitch, float iYaw)
	: mPitch(iPitch),
	mYaw(iYaw)
{
	glm::clamp(mPitch, -89.9f, 89.9f);

	// Find Direction from Pitch and Yaw
	glm::vec3 wFront;
	wFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	wFront.y = sin(glm::radians(mPitch));
	wFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));

	glm::vec3 wDir = glm::normalize(wFront);
	
	mView = std::make_unique<View>(Frustum::EProjectionType::ePerspective, wDir, iPos);
}

void Camera::SetFOV(float iFov)
{
	auto wFrustum = static_cast<PerspectiveFrustum*>(mView->GetFrustum());
	wFrustum->SetFOV(iFov);
}

void Camera::SetAspectRatio(float iAR)
{
	auto wFrustum = static_cast<PerspectiveFrustum*>(mView->GetFrustum());
	wFrustum->SetAspectRatio(iAR);
}

void Camera::SetNearPlane(float iNear)
{
	auto wFrustum = static_cast<PerspectiveFrustum*>(mView->GetFrustum());
	wFrustum->SetNearPlane(iNear);
}

void Camera::SetFarPlane(float iFar)
{
	auto wFrustum = static_cast<PerspectiveFrustum*>(mView->GetFrustum());
	wFrustum->SetFarPlane(iFar);
}

void Camera::Update(double iDeltaTime)
{
	if (Input::GetInstance()->IsMousePressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		Input::MouseCursorInfo wMouseCursorInfo{};
		Input::GetInstance()->GetMouseCursorInfo(wMouseCursorInfo);

		if (wMouseCursorInfo.isCursorMoved)
		{
			float wXoffset = static_cast<float>(wMouseCursorInfo.DeltaX) * mMouseSensitivity;
			float wYoffset = static_cast<float>(wMouseCursorInfo.DeltaY) * mMouseSensitivity;

			mYaw += wXoffset;
			mPitch += wYoffset;

			glm::clamp(mPitch, -89.9f, 89.9f);
		}

		glm::vec3 wFront;
		wFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		wFront.y = sin(glm::radians(mPitch));
		wFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));

		// normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		glm::vec3 wDirection = glm::normalize(wFront);
		glm::vec3 wRight = glm::normalize(glm::cross(wDirection, kWorldUp));
		glm::vec3 wUp = glm::normalize(glm::cross(wRight, wDirection));

		// Process Keyboard inputs
		float wDist = static_cast<float>(iDeltaTime) * mSpeed;
		if (Input::GetInstance()->IsKeyPressed(GLFW_KEY_W))
		{
			mView->GetTransform()->Translate(wDirection * wDist);
		}

		if (Input::GetInstance()->IsKeyPressed(GLFW_KEY_S))
		{
			mView->GetTransform()->Translate(-wDirection * wDist);
		}

		if (Input::GetInstance()->IsKeyPressed(GLFW_KEY_A))
		{
			mView->GetTransform()->Translate(-wRight * wDist);
		}

		if (Input::GetInstance()->IsKeyPressed(GLFW_KEY_D))
		{
			mView->GetTransform()->Translate(wRight * wDist);
		}

		if (Input::GetInstance()->IsKeyPressed(GLFW_KEY_E))
		{
			mView->GetTransform()->Translate(glm::vec3(0.f, wDist, 0.f));
		}

		if (Input::GetInstance()->IsKeyPressed(GLFW_KEY_Q))
		{
			mView->GetTransform()->Translate(glm::vec3(0.f, -wDist, 0.f));
		}

		mView->SetDirection(glm::normalize(wDirection));
	}
}

glm::mat4 Camera::ViewMatrix() const
{
	return mView->ViewMatrix();
}

glm::mat4 Camera::VPMatrix() const
{
	return mView->GetFrustum()->ProjectionMatrix() * mView->ViewMatrix();
}

const glm::vec3& Camera::WorldPos() const
{
	return mView->GetPos();
}
