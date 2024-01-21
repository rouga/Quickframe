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

#include <string>
#include <glad/glad.h>

class Framebuffer
{
public:
	Framebuffer(const std::string& iName, uint32_t iWidth, uint32_t iHeight);
	~Framebuffer();

	virtual bool Initialize() = 0;

	GLuint FramebufferHandle() const { return mFramebufferHandle; }
	GLuint TextureHandle() const { return mTextureHandle; }

	void BindSrc(GLenum iTextureUnit);
	void BindDst();
	void Unbind();

protected:
	GLuint mFramebufferHandle;
	GLuint mTextureHandle;

	std::string mName;
	GLuint mWidth;
	GLuint mHeight;
};

class ShadowMapFBO : public Framebuffer
{
public:
	ShadowMapFBO(const std::string& iName, uint32_t iWidth, uint32_t iHeight);

	bool Initialize() override;
};