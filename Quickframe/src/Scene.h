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

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

#include "Camera.h"
#include "Light.h"
#include "Skybox.h"

class Entity;

class Scene
{
	friend class SceneManager;
public:
	Scene(const std::string& iName);
	
	void Update(double iDeltaTime);

	const std::string& GetName() const { return mName; }

	void SetCamera(std::unique_ptr<Camera>& iCamera);
	Camera* GetCamera() const { return mCamera.get(); }

	void SetSkybox(std::unique_ptr<Skybox>& iSkybox);

	Skybox* GetSkybox() const { return mSkybox.get(); }

	void AddEntity(std::shared_ptr<Entity> iEntity);
	Entity* GetEntity(const std::string& iEntityName);
	const std::unordered_map<std::string, std::shared_ptr<Entity>>& GetEntities() const { return mEntities; }

	void SetDirectionalLight(std::unique_ptr<DirectionalLight>& iDirLight);
	void AddPointLight(std::unique_ptr<PointLight>& iPointLight);
	void AddSpotLight(std::unique_ptr<SpotLight>& iSpotLight);

	DirectionalLight* GetDirLight() const { return mDirLights.get(); }
	const PointLight* GetPointLight(uint32_t iIndex) const { return mPointLights[iIndex].get(); }
	const SpotLight* GetSpotLight(uint32_t iIndex) const { return mSpotLights[iIndex].get(); }

	uint32_t GetDirLightCount() const { return mDirLightCount; }
	uint32_t GetPointLightCount() const { return mPointLightCount; }
	uint32_t GetSpotLightCount() const { return mSpotLightCount; }

private:
	std::string mName;
	
	std::unique_ptr<DirectionalLight> mDirLights;
	std::vector<std::unique_ptr<PointLight>> mPointLights;
	std::vector<std::unique_ptr<SpotLight>> mSpotLights;

	uint32_t mPointLightCount = 0;
	uint32_t mDirLightCount = 0;
	uint32_t mSpotLightCount = 0;

	std::unique_ptr<Camera> mCamera;
	std::unique_ptr<Skybox> mSkybox;
	std::unordered_map<std::string, std::shared_ptr<Entity>> mEntities;
};