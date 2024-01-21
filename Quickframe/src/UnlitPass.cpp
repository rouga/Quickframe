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

#include "Engine.h"
#include "UnlitPass.h"
#include "Scene.h"

UnlitPass::UnlitPass()
{
	Shader wColorVertexShader("shaders/ColorVS.glsl", Shader::EShaderStage::eVertex);
	Shader wColorFragmentShader("shaders/ColorFS.glsl", Shader::EShaderStage::eFragment);
	std::vector<Shader> wShaders{wColorVertexShader, wColorFragmentShader};
	wShaders.push_back(wColorVertexShader);
	wShaders.push_back(wColorFragmentShader);
	mProgram = std::make_unique<Program>(wShaders);
}

void UnlitPass::Execute(Scene* iScene)
{
	uint32_t wWidth, wHeight;
	Engine::GetInstance()->GetWindow()->GetSize(wWidth, wHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, wWidth, wHeight);
	mProgram->Bind();
	for (uint32_t i = 0; i < iScene->GetPointLightCount(); i++)
	{
		const PointLight* wPointLight = iScene->GetPointLight(i);

		glm::vec3 wColor = wPointLight->GetColor();
		mProgram->SetUniform3f("uColor", wColor);

		glm::mat4 wTransform = iScene->GetCamera()->VPMatrix() * wPointLight->GetTransform()->GetWorldMatrix();
		mProgram->SetUniformMatrix4f("uMVP", wTransform);
		const SubMesh& wLightPointSubmesh = wPointLight->GetSubmesh();

		glBindVertexArray(wLightPointSubmesh.VertexArrayHandle());
		glEnableVertexAttribArray(SubMesh::EVertexAttrib::ePosition);
		glDrawElements(GL_TRIANGLES, wLightPointSubmesh.IndexCount(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(SubMesh::EVertexAttrib::ePosition);
	}
	mProgram->Unbind();
}

