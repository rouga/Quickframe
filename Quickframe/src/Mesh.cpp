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

#include <chrono>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <spdlog/spdlog.h>

#include "Mesh.h"
#include "Vertex.h"
#include "TextureManager.h"
#include "Transform.h"
#include "SubMesh.h"
#include "MeshNode.h"

using namespace std::chrono;

Mesh::Mesh()
{
}

bool Mesh::Load(const std::string& iFilename)
{
	auto wClockStart = high_resolution_clock::now();

	mDirectory = iFilename.substr(0, iFilename.find_last_of('/'));

	Assimp::Importer wImporter;
	const aiScene* wScene = wImporter.ReadFile(iFilename,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

	if (!wScene || wScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !wScene->mRootNode)
	{
		spdlog::critical("\tError while parsing mesh {0:s} : {1:s}", iFilename.c_str(), wImporter.GetErrorString());
		return false;
	}

	spdlog::info("\tLoading Mesh {0:s} : ", iFilename.c_str());

	mRootNode = std::make_unique<MeshNode>();
	mRootNode->mParent = nullptr;

	ProcessMeshNode(wScene->mRootNode, wScene, mRootNode.get());

	auto wClockStop = high_resolution_clock::now();
	auto wDuration = duration_cast<milliseconds>(wClockStop - wClockStart);
	spdlog::info("\tMesh {0:s} parsed ({1:d} Vertices | {2:d} Indices | {3:d} Nodes | {4:d} Submeshes) in {5:d} ms !",
		wScene->mRootNode->mName.C_Str(), mVertexCount, mIndexCount, mNodeCount, mSubmeshCount,  wDuration.count());

	return true;
}

void Mesh::ProcessMeshNode(aiNode* iNode, const aiScene* iScene, MeshNode* iMeshNode, uint32_t iNodeHierarchy)
{
	++mNodeCount;
	
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < iNode->mNumMeshes; i++)
	{
		iMeshNode->mSubmeshes.resize(iNode->mNumMeshes);
		aiMesh* wMesh = iScene->mMeshes[iNode->mMeshes[i]];
		CreateSubmesh(wMesh, iScene, iMeshNode->mSubmeshes[i], iNodeHierarchy);
		++mSubmeshCount;
	}
	
	iMeshNode->mChildrenNode.resize(iNode->mNumChildren);
	++iNodeHierarchy;
	iMeshNode->mHierarchyLevel = iNodeHierarchy;

	// then do the same for each of its children
	for (unsigned int i = 0; i < iNode->mNumChildren; i++)
	{
		iMeshNode->mChildrenNode[i].mParent = iMeshNode;
		ProcessMeshNode(iNode->mChildren[i], iScene, &iMeshNode->mChildrenNode[i], iNodeHierarchy);
	}
}

void Mesh::CreateSubmesh(aiMesh* iMesh, const aiScene* iScene, SubMesh& oSubmesh, uint32_t iNodeHierarchy)
{
	LoadSubmeshVertexAttribs(iMesh, iScene, oSubmesh);

	LoadSubmeshIndices(iMesh, iScene, oSubmesh);

	LoadSubmeshMaterial(iMesh, iScene, oSubmesh);

	oSubmesh.mName = iMesh->mName.length == 0 ? std::string("No Name") : std::string(iMesh->mName.C_Str());
	spdlog::info("{0:s}Created Submesh {1:s} with {2:d} vertices and {3:d} indices.",
		std::string(iNodeHierarchy + 2, '\t').c_str(), oSubmesh.mName.c_str(), iMesh->mNumVertices, iMesh->mNumFaces * 3);
}

void Mesh::LoadSubmeshMaterial(aiMesh* iMesh, const aiScene* iScene, SubMesh& oSubmesh)
{
	// Load Material
	const aiMaterial* wMaterial = iScene->mMaterials[iMesh->mMaterialIndex];
	if (!wMaterial)
	{
		spdlog::critical("Undefined Material For mesh {0:s}", iMesh->mName.C_Str());
	}
	oSubmesh.mMaterial = new Material;

	// Ambient Color
	aiColor3D wAmbientColor{ 0.f, 0.f, 0.f };
	if (wMaterial->Get(AI_MATKEY_COLOR_AMBIENT, wAmbientColor) == AI_SUCCESS)
	{
		oSubmesh.mMaterial->SetAmbientColor(glm::vec3(wAmbientColor.r, wAmbientColor.g, wAmbientColor.b));
	}

	// Diffuse Color
	aiColor3D wDiffuseColor{ 0.f, 0.f, 0.f };
	if (wMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, wDiffuseColor) == AI_SUCCESS)
	{
		oSubmesh.mMaterial->SetDiffuseColor(glm::vec3(wDiffuseColor.r, wDiffuseColor.g, wDiffuseColor.b));
	}

	// Specular Color
	aiColor3D wSpecularColor{ 0.f, 0.f, 0.f };
	if (wMaterial->Get(AI_MATKEY_COLOR_SPECULAR, wSpecularColor) == AI_SUCCESS)
	{
		oSubmesh.mMaterial->SetSpecularColor(glm::vec3(wSpecularColor.r, wSpecularColor.g, wSpecularColor.b));
	}

	// Diffuse Texture
	if (wMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString wPath;
		if (wMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &wPath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			std::string FullPath = mDirectory + "/" + wPath.data;
			oSubmesh.mMaterial->LoadDiffuseTex(FullPath);
		}
	}

	// Normal Texture
	if (wMaterial->GetTextureCount(aiTextureType_DISPLACEMENT) > 0)
	{
		aiString wPath;
		if (wMaterial->GetTexture(aiTextureType_DISPLACEMENT, 0, &wPath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			std::string FullPath = mDirectory + "/" + wPath.data;
			oSubmesh.mMaterial->LoadNormalTex(FullPath);
		}
	}

	if (wMaterial->GetTextureCount(aiTextureType_SHININESS) > 0)
	{
		aiString wPath;
		if (wMaterial->GetTexture(aiTextureType_SHININESS, 0, &wPath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			std::string FullPath = mDirectory + "/" + wPath.data;
			oSubmesh.mMaterial->LoadSpecularExponentTex(FullPath);
		}
	}
}

void Mesh::LoadSubmeshVertexAttribs(aiMesh* iMesh, const aiScene* iScene, SubMesh& oSubmesh)
{
	oSubmesh.mVertexCount = iMesh->mNumVertices;

	// Load Pos
	if (iMesh->mNumVertices > 0)
	{
		oSubmesh.mVertexAttribs[SubMesh::EVertexAttrib::ePosition] = true;
	}
	std::vector<glm::vec3> wVertices;
	wVertices.reserve(iMesh->mNumVertices);
	for (unsigned int wVertexID = 0; wVertexID < iMesh->mNumVertices; wVertexID++)
	{
		const aiVector3D& wPos = iMesh->mVertices[wVertexID];
		wVertices.emplace_back(glm::vec3(wPos.x, wPos.y, wPos.z));
	}

	glGenVertexArrays(1, &oSubmesh.mVAO);
	glBindVertexArray(oSubmesh.mVAO);
	glGenBuffers(1, &oSubmesh.mVBO[SubMesh::EVertexAttrib::ePosition]);
	glBindBuffer(GL_ARRAY_BUFFER, oSubmesh.mVBO[SubMesh::EVertexAttrib::ePosition]);
	glBufferData(GL_ARRAY_BUFFER, wVertices.size() * sizeof(glm::vec3), wVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(SubMesh::EVertexAttrib::ePosition);
	glVertexAttribPointer(SubMesh::EVertexAttrib::ePosition, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	// Load Normals
	if (iMesh->mNormals)
	{
		oSubmesh.mVertexAttribs[SubMesh::EVertexAttrib::eNormal] = true;
		std::vector<glm::vec3> wNormals;
		wNormals.reserve(iMesh->mNumVertices);
		for (unsigned int wVertexID = 0; wVertexID < iMesh->mNumVertices; wVertexID++)
		{
			const aiVector3D& wNormal = iMesh->mNormals[wVertexID];
			wNormals.emplace_back(glm::vec3(wNormal.x, wNormal.y, wNormal.z));
		}
		glGenBuffers(1, &oSubmesh.mVBO[SubMesh::EVertexAttrib::eNormal]);
		glBindBuffer(GL_ARRAY_BUFFER, oSubmesh.mVBO[SubMesh::EVertexAttrib::eNormal]);
		glBufferData(GL_ARRAY_BUFFER, wNormals.size() * sizeof(glm::vec3), wNormals.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(SubMesh::EVertexAttrib::eNormal);
		glVertexAttribPointer(SubMesh::EVertexAttrib::eNormal, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	}

	// Load UV0
	if (iMesh->HasTextureCoords(0))
	{
		oSubmesh.mVertexAttribs[SubMesh::EVertexAttrib::eUV0] = true;
		std::vector<glm::vec2> wUVs0;
		wUVs0.reserve(iMesh->mNumVertices);
		for (unsigned int wVertexID = 0; wVertexID < iMesh->mNumVertices; wVertexID++)
		{
			const aiVector3D& wUV0 = iMesh->mTextureCoords[0][wVertexID];
			wUVs0.emplace_back(glm::vec2(wUV0.x, wUV0.y));
		}
		glGenBuffers(1, &oSubmesh.mVBO[SubMesh::EVertexAttrib::eNormal]);
		glBindBuffer(GL_ARRAY_BUFFER, oSubmesh.mVBO[SubMesh::EVertexAttrib::eNormal]);
		glBufferData(GL_ARRAY_BUFFER, wUVs0.size() * sizeof(glm::vec2), wUVs0.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(SubMesh::EVertexAttrib::eUV0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
	}

	// Load Tangent
	if (oSubmesh.IsVertexAttribAvailable(SubMesh::eUV0) && oSubmesh.IsVertexAttribAvailable(SubMesh::eNormal))
	{
		oSubmesh.mVertexAttribs[SubMesh::EVertexAttrib::eTangent] = true;
		std::vector<glm::vec3> wTangents;
		wTangents.reserve(iMesh->mNumVertices);
		for (unsigned int wVertexID = 0; wVertexID < iMesh->mNumVertices; wVertexID++)
		{
			const aiVector3D& wTangent = iMesh->mTangents[wVertexID];
			wTangents.emplace_back(glm::vec3(wTangent.x, wTangent.y, wTangent.z));
		}
		glGenBuffers(1, &oSubmesh.mVBO[SubMesh::EVertexAttrib::eTangent]);
		glBindBuffer(GL_ARRAY_BUFFER, oSubmesh.mVBO[SubMesh::EVertexAttrib::eTangent]);
		glBufferData(GL_ARRAY_BUFFER, wTangents.size() * sizeof(glm::vec3), wTangents.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(SubMesh::EVertexAttrib::eTangent, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	}

	mVertexCount += static_cast<uint32_t>(wVertices.size());
}

void Mesh::LoadSubmeshIndices(aiMesh* iMesh, const aiScene* iScene, SubMesh& oSubmesh)
{
	oSubmesh.mIndexCount = iMesh->mNumFaces * 3;

	// Load Indices
	std::vector<GLuint> wIndices;
	wIndices.reserve(iMesh->mNumFaces * 3);
	for (unsigned int wFaceID = 0; wFaceID < iMesh->mNumFaces; wFaceID++)
	{
		assert(iMesh->mFaces[wFaceID].mNumIndices == 3);
		wIndices.push_back(iMesh->mFaces[wFaceID].mIndices[0]);
		wIndices.push_back(iMesh->mFaces[wFaceID].mIndices[1]);
		wIndices.push_back(iMesh->mFaces[wFaceID].mIndices[2]);
	}

	glGenBuffers(1, &oSubmesh.mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oSubmesh.mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, oSubmesh.mIndexCount * sizeof(GLuint), wIndices.data(), GL_STATIC_DRAW);

	mIndexCount += static_cast<uint32_t>(wIndices.size());
	glBindVertexArray(0);
}
