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

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#include "Renderer.h"
#include "Entity.h"
#include "Mesh.h"
#include "Camera.h"
#include "Texture.h"
#include "Transform.h"
#include "SubMesh.h"
#include "MeshNode.h"
#include "Scene.h"
#include "TextureManager.h"
#include "Defines.h"

Renderer::Renderer()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::Render(Scene* iScene)
{
	glClearColor(0.3f, 0.3f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mShadowPass->Execute(iScene);
	mUnlitPass->Execute(iScene);
	mLightPass->Execute(iScene);
	mSkyboxPass->Execute(iScene);
}

void Renderer::Initialize()
{
	mShadowPass = std::make_unique<ShadowPass>();
	mUnlitPass = std::make_unique<UnlitPass>();
	mLightPass = std::make_unique<LightPass>(mShadowPass.get());
	mSkyboxPass = std::make_unique<SkyboxPass>();
	

	TextureManager::GetInstance()->GetDefaultDiffuseTex();
}
