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

#include "Sphere.h"
#include "Transform.h"
#include "View.h"

class PerspectiveFrustum;
class OrthographicFrustum;

class Light
{
public:
	Light();
	Light(glm::vec3 mColor, float iIntensity);

	const glm::vec3& GetColor() const { return mColor; }
	void SetColor(const glm::vec3& iColor) { mColor = iColor; }

	float GetIntensity() const { return mIntensity; }
	void SetIntensity(float iIntensity) { mIntensity = iIntensity; }

	uint32_t GetID() const { return mID; }

	bool IsShadowEnabled() const { return mEnableShadow; }

protected:
	glm::vec3 mColor{ 1.0f, 1.0f, 1.0f };
	float mIntensity = 1.0f;
	uint32_t mID;
	bool mEnableShadow = true;

private:
	static int sID;
};


class DirectionalLight : public Light
{
public:
	DirectionalLight() {}
	DirectionalLight(float iIntensity, glm::vec3 iColor, glm::vec3 iDir, float iAmbientIntensity);

	const glm::vec3 GetDir() const { return mDir; }
	float GetAmbientIntensity() const { return mAmbientIntensity; }
	float GetAltitude() const { return mAltitude; }
	View* GetView() const { return mView.get(); }

	void SetAltitude(float iAlt);

private:

	std::unique_ptr<View> mView;
	OrthographicFrustum* mFrustum;
	glm::vec3 mDir{ 0.f, -1.f, 1.f };
	float mAmbientIntensity = 0.1f;
	float mAltitude = 100.0f;
};

class PointLight : public Light
{
public:
	struct Attenuation
	{
		float Constant = 1;
		float Linear = 0.005f;
		float Exp = 0.f;
	};

	PointLight();
	PointLight(float iIntensity, glm::vec3 iColor, glm::vec3 iPos, Attenuation iAttenuation);

	const glm::vec3 GetPos() const { return mTransform->GetPos(); }
	void SetPos(const glm::vec3& iPos) const { mTransform->SetWorldPos(iPos); }

	void SetRadius(float iRadius) { mTransform->SetScale(glm::vec3(iRadius)); }

	const Attenuation& GetAttenuation() const { return mAttenuation; }

	Transform* GetTransform() const { return mTransform.get(); }

	const SubMesh& GetSubmesh() const { return mSphere.GetMesh()->GetRootNode()->GetChildren()[0].GetSubmeshes()[0]; }

protected:
	std::shared_ptr<Transform> mTransform;
	Attenuation mAttenuation;
	Sphere mSphere;

};

class SpotLight : public PointLight
{
public:
	SpotLight();
	SpotLight(float iIntensity, glm::vec3 iColor, glm::vec3 iPos, Attenuation iAttenuation, glm::vec3 iDir, float iCutoff);

	virtual ~SpotLight(){}

	const glm::vec3& GetDirection() const { return mDirection; }
	void SetDirection(const glm::vec3& iDir);

	float GetCutoff() const { return mCutoff; }
	void SetCutoff(float iCutoff);

	View* GetView() const { return mView.get(); }
private: 

	glm::vec3 mDirection{ 0.f, -1.f, 1.f };
	float mCutoff = 0.93969f; // cos(20)

	std::unique_ptr<View> mView;
	PerspectiveFrustum* mFrustum;
};