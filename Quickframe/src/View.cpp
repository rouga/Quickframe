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

#include <glm/gtc/matrix_transform.hpp>

#include "View.h"
#include "PerspectiveFrustum.h"
#include "OrthographicFrustum.h"

View::View(Frustum::EProjectionType iProjectionType, glm::vec3 iDir, glm::vec3 iPos , std::shared_ptr<Transform> iTransform)
	:mDirection(iDir)
{
	if (iTransform)
	{
		mTransform = iTransform;
	}
	else
	{
		mTransform = std::make_shared<Transform>();
		mTransform->SetWorldPos(iPos);
	}

	switch (iProjectionType)
	{
	case Frustum::EProjectionType::eOrthographic:
		mFrustum = std::make_unique<OrthographicFrustum>();
		break;
	case Frustum::EProjectionType::ePerspective:
		mFrustum = std::make_unique<PerspectiveFrustum>();
		break;
	default:
		break;
	}
}

glm::mat4 View::ViewMatrix() const
{
	glm::vec3 wDirection = glm::normalize(mDirection);
	glm::vec3 wRight = glm::normalize(glm::cross(wDirection, kWorldUp));
	glm::vec3 wUp = glm::normalize(glm::cross(wRight, wDirection));

	return glm::lookAt(mTransform->GetPos(), mTransform->GetPos() + wDirection, wUp);
}
