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

#include "SceneManager.h"
#include "Entity.h"
#include "Light.h"
#include "Engine.h"
#include "Plane.h"
#include "Sphere.h"

SceneManager::SceneManager()
{
	LoadDefaultScene();
}

void SceneManager::LoadDefaultScene()
{
	auto wIt = mSceneMap.find("Default");
	if (wIt != mSceneMap.end())
	{
		return;
	}

	std::shared_ptr<Scene> wScene = std::make_shared<Scene>("Default");
	mSceneMap[wScene->GetName()] = wScene;
	mActiveScene = wScene;
	LoadDefaultCamera();
	LoadDefaultLight();
	LoadDefaultEntity();
	LoadDefaultSkybox();
}

void SceneManager::LoadDefaultLight()
{
	std::unique_ptr<DirectionalLight> wDirLight = std::make_unique<DirectionalLight>(0.8f,
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, -2.0f, -3.0f),
		0.4f);

	wDirLight->SetAltitude(50.f);

	mSceneMap["Default"]->SetDirectionalLight(wDirLight);

	std::unique_ptr<PointLight> wPointLight = std::make_unique<PointLight>();
	wPointLight->SetPos(glm::vec3(-3.0, 6.0f, -10.0f));
	wPointLight->SetRadius(0.05);
	wPointLight->SetColor(glm::vec3(1.0f, 0.f, 0.f));

	mSceneMap["Default"]->AddPointLight(wPointLight);
}

void SceneManager::LoadDefaultCamera()
{
	uint32_t wWndWidth, wWndHeight;
	Engine::GetInstance()->GetWindow()->GetSize(wWndWidth, wWndHeight);

	std::unique_ptr<Camera> mCamera = std::make_unique<Camera>(glm::vec3(0.f, 45.f, 0.f),
		-55.0f,
		0.0f
	);

	mCamera->SetAspectRatio((float)wWndWidth / (float)wWndHeight);
	mCamera->SetFOV(45);
	mCamera->SetNearPlane(0.1f);
	mCamera->SetFarPlane(1000.0f);

	mSceneMap["Default"]->SetCamera(mCamera);
}

void SceneManager::LoadDefaultEntity()
{
	std::shared_ptr<Plane> wPlane = std::make_shared<Plane>();
	wPlane->LoadMesh("resources/plane/Plane.obj");
	wPlane->SetWidth(30.f);
	wPlane->SetHeight(30.f);
	mSceneMap["Default"]->AddEntity(wPlane);

	std::shared_ptr<Entity> wTree = std::make_shared<Entity>("Tree");

	wTree->LoadMesh("resources/meshes/Lowpoly_tree.obj");
	wTree->SetPos(glm::vec3(8.f, 0.f, 16.f));
	wTree->GetTransform()->SetScale(glm::vec3(0.5));

	mSceneMap["Default"]->AddEntity(wTree);

	std::shared_ptr<Entity> wCottage = std::make_shared<Entity>("Cottage");

	wCottage->LoadMesh("resources/meshes/cottage/cottage.obj");
	wCottage->SetPos(glm::vec3(-8.f, 0.f, 0.f));
	wCottage->GetTransform()->SetScale(glm::vec3(0.5));

	mSceneMap["Default"]->AddEntity(wCottage);
}

void SceneManager::LoadDefaultSkybox()
{
	std::unique_ptr<Skybox> wSkybox = std::make_unique<Skybox>();

	std::vector<std::string> wFaces
	{
		  "resources/skybox/Standard/right.png",
			"resources/skybox/Standard/left.png",
			"resources/skybox/Standard/top.png",
			"resources/skybox/Standard/bottom.png",
			"resources/skybox/Standard/front.png",
			"resources/skybox/Standard/back.png"
	};

	wSkybox->LoadCubemap(wFaces);

	mSceneMap["Default"]->SetSkybox(wSkybox);
}
