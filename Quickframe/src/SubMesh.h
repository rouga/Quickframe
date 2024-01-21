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

#include <glad/glad.h>
#include <string>

#include "Material.h"

/**
 * @brief : a SubMesh contains all necessary informations (Vertex/Index Buffer handles, and materal) that
 * needs to be sent to GPU for rendering
*/

class SubMesh
{
	friend class Mesh;
public:

	enum EVertexAttrib
	{
		ePosition = 0,
		eNormal,
		eColor,
		eTangent,
		eUV0,
		eUV1,
		eUV2,
		eNumAttribs
	};

	bool IsVertexAttribAvailable(EVertexAttrib iVertexAttrib) const { return mVertexAttribs[iVertexAttrib]; }
	

	SubMesh() {};
	~SubMesh()
	{
		Free();
	}

	void Free();

	Material* GetMaterial() const { return mMaterial; }

	GLuint VertexArrayHandle() const { return mVAO; }
	GLuint VertexBufferHandle(EVertexAttrib iVertexAttrib ) const { return mVBO[iVertexAttrib]; }
	GLuint IndexBufferHandle() const { return mIBO; }
	GLuint IndexCount() const { return mIndexCount; }

private:
	bool mVertexAttribs[eNumAttribs] = { false };

	Material* mMaterial = nullptr;

	GLuint mVAO = 0;
	GLuint mIBO = 0;
	GLuint mVBO[EVertexAttrib::eNumAttribs] = { 0 };
	GLuint mIndexCount = 0;
	GLuint mVertexCount = 0;

	std::string mName;
};