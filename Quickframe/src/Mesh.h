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

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>

#include "MeshNode.h"

class Transform;

class Texture;
class SubMesh;
struct aiMesh;
struct aiScene;
struct aiNode;

/**
 * @brief Mesh class holds the tree structure for a Loaded mesh
 * Each Mesh is composed of MeshNodes, which contain subMeshes.
*/
class Mesh
{
public:
	Mesh();

	void Free() {}

	~Mesh() {}

	bool Load(const std::string& iFilename);

	const MeshNode* GetRootNode() const { return mRootNode.get(); }

private:
	/**
	 * @brief 
	 * @param iNode : Assimp Node Object
	 * @param iScene : Assimp Scene Object
	 * @param iMeshNode : MeshNode Object 
	 * @param iHierarchyLevel 
	*/
	void ProcessMeshNode(aiNode* iNode, const aiScene* iScene, MeshNode* iMeshNode, uint32_t iHierarchyLevel = 0);

	void CreateSubmesh(aiMesh* iMesh, const aiScene* iScene, SubMesh& oSubmesh, uint32_t iNodeHierarchy);

	void LoadSubmeshMaterial(aiMesh* iMesh, const aiScene* iScene, SubMesh& oSubmesh);

	void LoadSubmeshVertexAttribs(aiMesh* iMesh, const aiScene* iScene, SubMesh& oSubmesh);

	void LoadSubmeshIndices(aiMesh* iMesh, const aiScene* iScene, SubMesh& oSubmesh);

	std::unique_ptr<MeshNode> mRootNode = nullptr;
	std::string mDirectory;
	std::string mName;
	uint32_t mNodeCount = 0;
	uint32_t mIndexCount = 0;
	uint32_t mVertexCount = 0;
	uint32_t mSubmeshCount = 0;
};