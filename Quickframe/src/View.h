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
#include <glm/vec3.hpp>
#include "Frustum.h"
#include "Transform.h"

class View
{
public:
	View(Frustum::EProjectionType iProjectionType, glm::vec3 iDir, glm::vec3 iPos, std::shared_ptr<Transform> iTransform = nullptr);

	const glm::vec3& GetPos() { return mTransform->GetPos(); }
	void SetPos(const glm::vec3& iPos) { mTransform->SetWorldPos(iPos);};

	const glm::vec3& GetDirection() const { return mDirection; }
	void SetDirection(const glm::vec3& iDir) { mDirection = iDir; };

	Frustum* GetFrustum() const { return mFrustum.get(); }

	glm::mat4 ViewMatrix() const;

	Transform* GetTransform() const
	{
		return mTransform.get();
	}

private:
	std::unique_ptr<Frustum> mFrustum;
	std::shared_ptr<Transform> mTransform;
	glm::vec3 mDirection;

 const glm::vec3 kWorldUp{0.f, 1.f, 0.0};
};