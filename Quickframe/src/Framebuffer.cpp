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

#include <spdlog/spdlog.h>

#include "Framebuffer.h"

Framebuffer::Framebuffer(const std::string& iName ,uint32_t iWidth, uint32_t iHeight)
	:mName(iName),
	mWidth(iWidth),
	mHeight(iHeight)
{
}

Framebuffer::~Framebuffer()
{
	glDeleteTextures(1, &mTextureHandle);
	glDeleteFramebuffers(1, &mFramebufferHandle);
}

void Framebuffer::BindSrc(GLenum iTextureUnit)
{
	glActiveTexture(iTextureUnit);
	glBindTexture(GL_TEXTURE_2D, mTextureHandle);
}

void Framebuffer::BindDst()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferHandle);
	glViewport(0, 0, mWidth, mHeight);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowMapFBO::ShadowMapFBO(const std::string& iName, uint32_t iWidth, uint32_t iHeight)
	:Framebuffer(iName, iWidth, iHeight)
{
}

bool ShadowMapFBO::Initialize()
{
	// Generate Framebuffer handle
	glGenFramebuffers(1, &mFramebufferHandle);

	glGenTextures(1, &mTextureHandle);
	glBindTexture(GL_TEXTURE_2D, mTextureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mWidth, mHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float border = 1.0f;
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &border);

	glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferHandle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mTextureHandle, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum wStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (wStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		spdlog::critical("Error While Creating Framebuffer {0:s} !", mName);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}
