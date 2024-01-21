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

#include <stb_image.h>
#include <spdlog/spdlog.h>
#include "Texture.h"

Texture::Texture(GLenum iTextureTarget)
	:mTextureTarget(iTextureTarget)
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &mTextureHandle);
}

bool Texture::Load(const std::string& iPath)
{
	mPath = iPath;
	stbi_set_flip_vertically_on_load(1);

	// Load texture to Host Memory
	unsigned char* wTextureData = stbi_load(mPath.c_str(), &mWidth, &mHeight, &mBpp, 0);
	if (!wTextureData)
	{
		spdlog::critical("Failed to load texture {0:s} : {1:s}", mPath.c_str(), stbi_failure_reason());
		return false;
	}

	// Load Texture to Device Memory
	glGenTextures(1, &mTextureHandle);
	glBindTexture(mTextureTarget, mTextureHandle);
	
	GLuint wPixelDataFormat = GL_RGB;
	switch (mBpp)
	{
	case 1:
		wPixelDataFormat = GL_R8;
		break;
	case 2:
		wPixelDataFormat = GL_RG;
		break;
	case 3:
		wPixelDataFormat = GL_RGB;
		break;
	case 4:
		wPixelDataFormat = GL_RGBA;
		break;
	}
	if (mTextureTarget == GL_TEXTURE_2D)
	{
		glTexImage2D(mTextureTarget, 0, GL_RGB, mWidth, mHeight, 0, wPixelDataFormat, GL_UNSIGNED_BYTE, wTextureData);
	}
	
	glGenerateMipmap(GL_TEXTURE_2D);

	// Sampler Parameters
	glTexParameteri(mTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(mTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	

	glBindTexture(mTextureTarget, 0);	

	spdlog::info("Texture {0:s} Loaded to Device (Width : {1:d} | Height : {2:d} )",
		mPath.c_str(), mWidth, mHeight);

	stbi_image_free(wTextureData);

	return true;
}

bool Texture::LoadCubemap(const std::vector<std::string>& iPaths)
{
	mPath = iPaths[0];
	stbi_set_flip_vertically_on_load(0);

	glGenTextures(1, &mTextureHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureHandle);

	for (unsigned int i = 0; i < iPaths.size(); i++)
	{
		unsigned char* wData = stbi_load(iPaths[i].c_str(), &mWidth, &mHeight, &mBpp, 0);

		if (!wData)
		{
			spdlog::critical("Failed to load texture {0:s} : {1:s}", iPaths[i].c_str(), stbi_failure_reason());
			return false;
		}

		GLuint wPixelDataFormat = GL_RGB;
		switch (mBpp)
		{
		case 1:
			wPixelDataFormat = GL_RED;
		case 2:
			wPixelDataFormat = GL_RG;
			break;
		case 3:
			wPixelDataFormat = GL_RGB;
			break;
		case 4:
			wPixelDataFormat = GL_RGBA;
			break;
		}

		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			GL_RGB,
			mWidth,
			mHeight,
			0,
			wPixelDataFormat,
			GL_UNSIGNED_BYTE,
			wData);

		stbi_image_free(wData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	spdlog::info("Cubemap texture {0:s} Loaded to Device (Width : {1:d} | Height : {2:d} )",
		mPath.c_str(), mWidth, mHeight);

	return true;
}

void Texture::Bind(GLenum iTextureUnit)
{
	glActiveTexture(iTextureUnit);
	glBindTexture(mTextureTarget, mTextureHandle);
}

