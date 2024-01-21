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

#include "SkyboxPass.h"
#include "TextureManager.h"
#include "Scene.h"
#include "Defines.h"

SkyboxPass::SkyboxPass()
{
	Shader wVertexShader("shaders/SkyboxVS.glsl", Shader::EShaderStage::eVertex);
	Shader wFragmentShader("shaders/SkyboxFS.glsl", Shader::EShaderStage::eFragment);

	std::vector<Shader> wShaders{wVertexShader, wFragmentShader};
	mProgram = std::make_unique<Program>(wShaders);
}

void SkyboxPass::Execute(Scene* iScene)
{
	if (Skybox* wSkybox = iScene->GetSkybox())
	{
		GLint wCurrentCullFaceMode;
		glGetIntegerv(GL_CULL_FACE_MODE, &wCurrentCullFaceMode);
		GLint wCurrentDepthFuncMode;
		glGetIntegerv(GL_DEPTH_FUNC, &wCurrentDepthFuncMode);

		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);

		mProgram->Bind();

		glm::mat4 wView =  glm::mat4(glm::mat3(iScene->GetCamera()->ViewMatrix()));
		glm::mat4 wVP = iScene->GetCamera()->ProjectionMatrix() * wView;
		mProgram->SetUniformMatrix4f("uVP", wVP);

		wSkybox->GetCubemap()->Bind(CUBEMAP_0_TEXTURE_UNIT);
		mProgram->SetUniform1i(CUBEMAP_0_TEXTURE_UNIFORM, CUBEMAP_0_TEXTURE_UNIFORM_IDX);
		
		const SubMesh& wSubmesh = wSkybox->GetSubmesh();
		glBindVertexArray(wSubmesh.VertexArrayHandle());
		glEnableVertexAttribArray(SubMesh::EVertexAttrib::ePosition);

		glDrawElements(GL_TRIANGLES, wSubmesh.IndexCount(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(SubMesh::EVertexAttrib::ePosition);

		mProgram->Unbind();

		glCullFace(wCurrentCullFaceMode);
		glDepthFunc(wCurrentDepthFuncMode);
	}
}

