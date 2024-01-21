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


class Texture
{
public:
	Texture(GLenum iTextureTarget);
	~Texture();

	/**
	 * @brief : Loads the file texture data to the CPU (RAM) then GPU (VRAM)
	 * @return whether the operation is successful
	*/
	bool Load(const std::string& iPath);

	/**
	* @brief : Loads cubemap composed of 6 iamges to the CPU (RAM) then GPU (VRAM)
	* They should be all of the same size
	* @return whether the operation is successful
	*/
	bool LoadCubemap(const std::vector<std::string>& iPath);

	/**
	 * @brief : Bind the texture object to a specific slot in the shader
	 * @param iTextureUnit : Slot number
	*/
	void Bind(GLenum iTextureUnit);

	const std::string& GetPath() { return mPath; }

private:
	GLuint mTextureHandle = 0;
	GLenum mTextureTarget;
	std::string mPath;
	int mWidth = 0;
	int mHeight = 0;
	int mDepth = 0;
	int mBpp = 0;
};