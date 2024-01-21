
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
#include "Program.h"
#include "LightPass.h"
#include "UnlitPass.h"
#include "ShadowPass.h"
#include "SkyboxPass.h"

class Mesh;
class Camera;
class Entity;
class MeshNode;
class Scene;

class Renderer
{
public:
	Renderer();
	void Render(Scene* iScene);
	
	void Initialize();

private:
	std::unique_ptr<ShadowPass> mShadowPass;
	std::unique_ptr<LightPass> mLightPass;
	std::unique_ptr<UnlitPass> mUnlitPass;
	std::unique_ptr<SkyboxPass> mSkyboxPass;
};