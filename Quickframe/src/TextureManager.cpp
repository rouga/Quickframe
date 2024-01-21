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

#include "TextureManager.h"

TextureManager* TextureManager::mTextureManager = nullptr;

TextureManager* TextureManager::GetInstance()
{
	if (!mTextureManager)
	{
		mTextureManager = new TextureManager();
	}

	return mTextureManager;
}

Texture* TextureManager::GetTexture(GLenum iTextureTarget, const std::string& iName)
{
	auto wTextureIter = mTextureMap.find(iName);
	if (wTextureIter != mTextureMap.end())
	{
		return mTextureMap[iName];
	}
	else
	{
		Texture* wTexture = new Texture(iTextureTarget);
		if (!wTexture->Load(iName))
		{
			return nullptr;
		}
		else
		{
			mTextureMap[wTexture->GetPath()] = wTexture;
			return mTextureMap[wTexture->GetPath()];
		}
	}
}

Texture* TextureManager::GetTexture(GLenum iTextureTarget, const std::vector<std::string>& iName)
{
	auto wTextureIter = mTextureMap.find(iName[0]);
	if (wTextureIter != mTextureMap.end())
	{
		return mTextureMap[iName[0]];
	}
	else
	{
		Texture* wTexture = new Texture(iTextureTarget);
		if (!wTexture->LoadCubemap(iName))
		{
			return nullptr;
		}
		else
		{
			mTextureMap[wTexture->GetPath()] = wTexture;
			return mTextureMap[wTexture->GetPath()];
		}
	}
}

Texture* TextureManager::GetDefaultDiffuseTex()
{
	return GetTexture(GL_TEXTURE_2D, "resources/textures/pattern.png");
}

