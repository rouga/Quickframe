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

#include <spdlog/spdlog.h>

#include "Scene.h"
#include "Entity.h"
#include "Light.h"
#include "Camera.h"
#include <GLFW/glfw3.h>

Scene::Scene(const std::string& iName)
	:mName(iName)
{
}

void Scene::Update(double iDeltaTime)
{
	mCamera->Update(iDeltaTime);

	//for (auto& wMap : mEntities)
	//{
	//	wMap.second->GetTransform()->Rotate(glm::vec3(0.f, 90.f * iDeltaTime, 0.0f));
	//}
}

void Scene::SetCamera(std::unique_ptr<Camera>& iCamera)
{
	mCamera = std::move(iCamera);
}

void Scene::SetSkybox(std::unique_ptr<Skybox>& iSkybox)
{
	mSkybox = std::move(iSkybox);
}

void Scene::AddEntity(std::shared_ptr<Entity> iEntity)
{
	mEntities[iEntity->GetName()] = iEntity;
}

void Scene::SetDirectionalLight(std::unique_ptr<DirectionalLight>& iDirLight)
{
	mDirLights = std::move(iDirLight);
	mDirLightCount++;
}

void Scene::AddPointLight(std::unique_ptr<PointLight>& iPointLight)
{
	mPointLights.push_back(std::move(iPointLight));
	mPointLightCount++;
}

void Scene::AddSpotLight(std::unique_ptr<SpotLight>& iSpotLight)
{
	mSpotLights.push_back(std::move(iSpotLight));
	mSpotLightCount++;
}

Entity* Scene::GetEntity(const std::string& iEntityName)
{
	auto wEntity = mEntities.find(iEntityName);
	if (wEntity != mEntities.end())
	{
		return wEntity->second.get();
	}
	return nullptr;
}
