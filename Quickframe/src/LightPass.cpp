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

#include <glm/vec3.hpp>

#include "Engine.h"
#include "LightPass.h"
#include "Scene.h"
#include "Texture.h"
#include "Defines.h"
#include "MeshNode.h"
#include "ShadowPass.h"

LightPass::LightPass(ShadowPass* iShadowPass)
	:mShadowPass(iShadowPass)
{
	Shader wTextureVertexShader("shaders/BlinnPhongVS.glsl", Shader::EShaderStage::eVertex);
	Shader wTextureFragmentShader("shaders/BlinnPhongFS.glsl", Shader::EShaderStage::eFragment);

	std::vector<Shader> wShaders{wTextureVertexShader, wTextureFragmentShader};
	mProgram = std::make_unique<Program>(wShaders);	
}

void LightPass::Execute(Scene* iScene)
{

	uint32_t wWidth, wHeight;
	Engine::GetInstance()->GetWindow()->GetSize(wWidth, wHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, wWidth, wHeight);

	// Send Uniforms
	mProgram->Bind();

	SetLightUniforms(iScene);
	mProgram->SetUniform3f("uCameraWorldPos", iScene->GetCamera()->WorldPos());

	for (auto wEntityMap : iScene->GetEntities())
	{
		Entity* wEntity = wEntityMap.second.get();
		glm::mat4 wTransform = iScene->GetCamera()->VPMatrix() * wEntity->GetTransform()->GetWorldMatrix();
		glm::mat4 wWorldMatrix = wEntity->GetTransform()->GetWorldMatrix();
		mProgram->SetUniformMatrix4f("uMVP", wTransform);
		mProgram->SetUniformMatrix4f("uWorld", wWorldMatrix);
		mProgram->SetUniformMatrix3f("uNormalMatrix", glm::transpose(glm::inverse(wWorldMatrix)));
		mProgram->SetUniformMatrix4f("uLightMVP", iScene->GetDirLight()->GetView()->GetFrustum()->ProjectionMatrix() *
			iScene->GetDirLight()->GetView()->ViewMatrix() * wWorldMatrix);

		const MeshNode* wEntityRoot = wEntity->GetMesh()->GetRootNode();
		if (wEntityRoot)
		{
			RenderMeshNode(*wEntityRoot);
		}
	}

	mProgram->Unbind();
}

void LightPass::SetLightUniforms(Scene* iScene)
{
	mProgram->SetUniform1i("uDirLightNum", iScene->GetDirLightCount());
	mProgram->SetUniform1i("uShadowEnabled", iScene->GetDirLight()->IsShadowEnabled());
	
	if (iScene->GetDirLight())
	{
		DirectionalLight* wDirLight = iScene->GetDirLight();
		mProgram->SetUniform1f("uDirLight.Intensity", wDirLight->GetIntensity());
		mProgram->SetUniform3f("uDirLight.Color", wDirLight->GetColor());
		mProgram->SetUniform3f("uDirLight.Dir", glm::normalize(wDirLight->GetDir()));
		mProgram->SetUniform1f("uDirLight.AmbientIntensity", wDirLight->GetAmbientIntensity());

		if (wDirLight->IsShadowEnabled())
		{
			Framebuffer* wShadowMapFBO = mShadowPass->GetShadowFBOMap().at(wDirLight->GetID()).get();
			wShadowMapFBO->BindSrc(SHADOW_MAP_0_TEXTURE_UNIT);
			mProgram->SetUniform1i(SHADOW_MAP_0_TEXTURE_UNIFORM, SHADOW_MAP_0_TEXTURE_UNIFORM_IDX);
		}
	}

	mProgram->SetUniform1i("uPointLightNum", iScene->GetPointLightCount());
	for (uint32_t i = 0; i < iScene->GetPointLightCount(); i++)
	{
		char wUniformChar[128];
		memset(wUniformChar, 0, sizeof(wUniformChar));

		_snprintf_s(wUniformChar, sizeof(wUniformChar), "uPointLight[%d].Intensity", i);
		mProgram->SetUniform1f(std::string(wUniformChar), iScene->GetPointLight(i)->GetIntensity());

		_snprintf_s(wUniformChar, sizeof(wUniformChar), "uPointLight[%d].Color", i);
		mProgram->SetUniform3f(std::string(wUniformChar), iScene->GetPointLight(i)->GetColor());

		_snprintf_s(wUniformChar, sizeof(wUniformChar), "uPointLight[%d].Position", i);
		mProgram->SetUniform3f(std::string(wUniformChar), iScene->GetPointLight(i)->GetPos());

		_snprintf_s(wUniformChar, sizeof(wUniformChar), "uPointLight[%d].Atten.Constant", i);
		mProgram->SetUniform1f(std::string(wUniformChar), iScene->GetPointLight(i)->GetAttenuation().Constant);

		_snprintf_s(wUniformChar, sizeof(wUniformChar), "uPointLight[%d].Atten.Linear", i);
		mProgram->SetUniform1f(std::string(wUniformChar), iScene->GetPointLight(i)->GetAttenuation().Linear);

		_snprintf_s(wUniformChar, sizeof(wUniformChar), "uPointLight[%d].Atten.Exp", i);
		mProgram->SetUniform1f(std::string(wUniformChar), iScene->GetPointLight(i)->GetAttenuation().Linear);
	}

	mProgram->SetUniform1i("uSpotLightNum", iScene->GetSpotLightCount());
	for (uint32_t i = 0; i < iScene->GetSpotLightCount(); i++)
	{
		const SpotLight* wSpotLight = iScene->GetSpotLight(i);
		char wUniformChar[128];
		memset(wUniformChar, 0, sizeof(wUniformChar));

		_snprintf_s(wUniformChar, sizeof(wUniformChar), "uSpotLight[%d].PL.Intensity", i);
		mProgram->SetUniform1f(std::string(wUniformChar), wSpotLight->GetIntensity());

		_snprintf_s(wUniformChar, sizeof(wUniformChar), "uSpotLight[%d].PL.Color", i);
		mProgram->SetUniform3f(std::string(wUniformChar), wSpotLight->GetColor());

		_snprintf_s(wUniformChar, sizeof(wUniformChar), "uSpotLight[%d].PL.Position", i);
		mProgram->SetUniform3f(std::string(wUniformChar), wSpotLight->GetPos());

		_snprintf_s(wUniformChar, sizeof(wUniformChar), "uSpotLight[%d].PL.Atten.Constant", i);
		mProgram->SetUniform1f(std::string(wUniformChar), wSpotLight->GetAttenuation().Constant);

		_snprintf_s(wUniformChar, sizeof(wUniformChar), "uSpotLight[%d].PL.Atten.Linear", i);
		mProgram->SetUniform1f(std::string(wUniformChar), wSpotLight->GetAttenuation().Linear);

		_snprintf_s(wUniformChar, sizeof(wUniformChar), "uSpotLight[%d].PL.Atten.Exp", i);
		mProgram->SetUniform1f(std::string(wUniformChar), wSpotLight->GetAttenuation().Linear);

		_snprintf_s(wUniformChar, sizeof(wUniformChar), "uSpotLight[%d].Cutoff", i);
		mProgram->SetUniform1f(std::string(wUniformChar), wSpotLight->GetCutoff());

		_snprintf_s(wUniformChar, sizeof(wUniformChar), "uSpotLight[%d].Dir", i);
		mProgram->SetUniform3f(std::string(wUniformChar), wSpotLight->GetDirection());
	}
}

void LightPass::RenderMeshNode(const MeshNode& iMeshNode)
{
	for (const auto& wSubMesh : iMeshNode.GetSubmeshes())
	{
		glBindVertexArray(wSubMesh.VertexArrayHandle());
		glEnableVertexAttribArray(SubMesh::EVertexAttrib::ePosition);
		glEnableVertexAttribArray(SubMesh::EVertexAttrib::eNormal);

		mProgram->SetUniform1i("uColorTexAvailable", 0);
		mProgram->SetUniform1i("uSpecularTexAvailable", 0);
		mProgram->SetUniform1i("uNormalTexAvailable", 0);

		if (wSubMesh.GetMaterial()->GetDiffuseTex())
		{
			mProgram->SetUniform1i("uColorTexAvailable", 1);
			glEnableVertexAttribArray(SubMesh::EVertexAttrib::eUV0);
			Material* wSubMeshMaterial = wSubMesh.GetMaterial();
			wSubMeshMaterial->GetDiffuseTex()->Bind(COLOR_TEXTURE_UNIT);
			mProgram->SetUniform1i(COLOR_TEXTURE_UNIFORM, COLOR_TEXTURE_UNIFORM_IDX);
		}

		if (wSubMesh.GetMaterial()->GetNormalTex())
		{
			mProgram->SetUniform1i("uNormalTexAvailable", 1);
			glEnableVertexAttribArray(SubMesh::EVertexAttrib::eTangent);
			Material* wSubMeshMaterial = wSubMesh.GetMaterial();
			wSubMeshMaterial->GetNormalTex()->Bind(NORMAL_TEXTURE_UNIT);
			mProgram->SetUniform1i(NORMAL_TEXTURE_UNIFORM, NORMAL_TEXTURE_UNIFORM_IDX);
		}

		if (wSubMesh.GetMaterial()->GetSpecularExponentTex())
		{
			mProgram->SetUniform1i("uSpecularTexAvailable", 1);
			Material* wSubMeshMaterial = wSubMesh.GetMaterial();
			wSubMeshMaterial->GetDiffuseTex()->Bind(SPECULAR_EXPONENT_TEXTURE_UNIT);
			mProgram->SetUniform1i(SPECULAR_EXPONENT_TEXTURE_UNIFORM, SPECULAR_EXPONENT_TEXTURE_UNIFORM_IDX);
		}

		mProgram->SetUniform3f("uMaterial.Ambient", wSubMesh.GetMaterial()->GetAmbientColor());
		mProgram->SetUniform3f("uMaterial.Diffuse", wSubMesh.GetMaterial()->GetDiffuseColor());
		mProgram->SetUniform3f("uMaterial.Specular", wSubMesh.GetMaterial()->GetSpecularColor());
		mProgram->SetUniform1f("uMaterial.SpecularExponent", wSubMesh.GetMaterial()->GetSpecularExponent());

		glDrawElements(GL_TRIANGLES, wSubMesh.IndexCount(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(SubMesh::EVertexAttrib::ePosition);
		glDisableVertexAttribArray(SubMesh::EVertexAttrib::eUV0);

	}

	for (auto& wChildren : iMeshNode.GetChildren())
	{
		RenderMeshNode(wChildren);
	}
}
