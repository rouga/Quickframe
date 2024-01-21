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
#include <string>

#include "Mesh.h"
#include "Transform.h"

class Entity
{
public:
	Entity(const std::string& iName);

	bool LoadMesh(const std::string& iPath);
	Mesh* GetMesh() const { return mMesh.get(); }
	Transform* GetTransform() const { return mTransform.get(); }

	void SetPos(const glm::vec3& iPos) { mTransform->SetWorldPos(iPos); }
	const glm::vec3& GetPos() { return mTransform->GetPos(); }

	const std::string& GetName() const { return mName; }

protected:
	std::unique_ptr<Transform> mTransform;
	std::string mName;
	std::unique_ptr<Mesh> mMesh;
};