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

#include "ShadowPass.h"
#include "Scene.h"
#include "MeshNode.h"

ShadowPass::ShadowPass()
{
	Shader wVertexShader("shaders/ShadowMapVS.glsl", Shader::EShaderStage::eVertex);
	Shader wFragmentShader("shaders/ShadowMapFS.glsl", Shader::EShaderStage::eFragment);

	std::vector<Shader> wShaders{wVertexShader, wFragmentShader};
	mProgram = std::make_unique<Program>(wShaders);
}

void ShadowPass::Execute(Scene* iScene)
{
	mProgram->Bind();
	for (uint32_t i = 0; i < iScene->GetDirLightCount(); i++)
	{
		ShadowMapFBO* wFramebuffer = nullptr;
		const DirectionalLight* wDirLight = iScene->GetDirLight();

		if (wDirLight->IsShadowEnabled())
		{
			auto wIt = mFramebufferMap.find(wDirLight->GetID());

			if (wIt != mFramebufferMap.end())
			{
				wFramebuffer = mFramebufferMap[wDirLight->GetID()].get();
			}
			else
			{
				mFramebufferMap[wDirLight->GetID()] = std::make_unique<ShadowMapFBO>(
					"Shadow Map Light " + std::to_string(wDirLight->GetID()),
					2048,
					2048);

				wFramebuffer = mFramebufferMap[wDirLight->GetID()].get();

				wFramebuffer->Initialize();
			}
			wFramebuffer->BindDst();
			glClear(GL_DEPTH_BUFFER_BIT);

			for (auto wEntityMap : iScene->GetEntities())
			{
				Entity* wEntity = wEntityMap.second.get();

				glm::mat4 wTransform = wDirLight->GetView()->GetFrustum()->ProjectionMatrix() *
					wDirLight->GetView()->ViewMatrix() *
					wEntity->GetTransform()->GetWorldMatrix();

				mProgram->SetUniformMatrix4f("uMVP", wTransform);

				const MeshNode* wEntityRoot = wEntity->GetMesh()->GetRootNode();
				if (wEntityRoot)
				{
					RenderMeshNode(*wEntityRoot);
				}
			}
		}

		wFramebuffer->Unbind();
	}
	mProgram->Unbind();
}

void ShadowPass::RenderMeshNode(const MeshNode& iMeshNode)
{
	for (const auto& wSubMesh : iMeshNode.GetSubmeshes())
	{
		glBindVertexArray(wSubMesh.VertexArrayHandle());
		glEnableVertexAttribArray(SubMesh::EVertexAttrib::ePosition);

		glDrawElements(GL_TRIANGLES, wSubMesh.IndexCount(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(SubMesh::EVertexAttrib::ePosition);
	}

	for (auto& wChildren : iMeshNode.GetChildren())
	{
		RenderMeshNode(wChildren);
	}
}

