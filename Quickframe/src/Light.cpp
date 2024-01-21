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

#include "Light.h"
#include "Transform.h"
#include "PerspectiveFrustum.h"
#include "OrthographicFrustum.h"

int Light::sID = 0;

Light::Light()
{
	sID++;
}

Light::Light(glm::vec3 iColor, float iIntensity)
	:Light()
{
	mColor = iColor;
	mIntensity = iIntensity;
	mID = sID;
	sID++;
}

DirectionalLight::DirectionalLight(float iIntensity, glm::vec3 iColor, glm::vec3 iDir, float iAmbientIntensity)
	: Light(iColor, iIntensity)
{
	mDir = iDir;
	mAmbientIntensity = iAmbientIntensity;
	glm::vec3 wPos = -glm::normalize(iDir) * mAltitude;
	mView = std::make_unique<View>(Frustum::EProjectionType::eOrthographic, mDir, wPos);
}

void DirectionalLight::SetAltitude(float iAlt)
{
	mAltitude = iAlt;
}

PointLight::PointLight()
{
	mTransform = std::make_shared<Transform>();
}

PointLight::PointLight(float iIntensity, glm::vec3 iColor, glm::vec3 iPosition, Attenuation iAttenuation)
	:Light(iColor, iIntensity)
{
	mIntensity = iIntensity;
	mColor = iColor;

	mTransform = std::make_shared<Transform>();
	mTransform->SetWorldPos(iPosition);
}

SpotLight::SpotLight()
{
	mView = std::make_unique<View>(Frustum::EProjectionType::ePerspective, mDirection, glm::vec3(0, 0, 0), mTransform);
	mFrustum = static_cast<PerspectiveFrustum*>(mView->GetFrustum());
	mFrustum->SetFOV(glm::degrees(acos(mCutoff)) * 2.f);
}

SpotLight::SpotLight(float iIntensity, glm::vec3 iColor, glm::vec3 iPos, Attenuation iAttenuation, glm::vec3 iDir, float iCutoff)
	:PointLight(iIntensity, iColor, iPos, iAttenuation)
{
	mDirection = iDir;
	mCutoff = iCutoff;
	mView = std::make_unique<View>(Frustum::EProjectionType::ePerspective, mDirection, glm::vec3(0, 0, 0), mTransform);
	mFrustum = static_cast<PerspectiveFrustum*>(mView->GetFrustum());
	mFrustum->SetFOV(glm::degrees(acos(mCutoff)) * 2.f);
}

void SpotLight::SetDirection(const glm::vec3& iDir)
{
	mDirection = iDir;
	mView->SetDirection(iDir);
}

void SpotLight::SetCutoff(float iCutoff)
{
	mCutoff = iCutoff;
	mFrustum->SetFOV(glm::degrees(acos(mCutoff)) * 2.f);
}