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

class Window
{
public:
	
	/**
	 * @brief Struct for window creation
	*/
	struct WindowInfo
	{
		uint32_t mWidth;
		uint32_t mHeight;
		std::string mTitle;
		bool mFullscreen;
	};

	Window(const WindowInfo& iWindowInfo);

	/**
	 * @brief Get the internal representation of a GLFW window
	 * @return Pointer to a GLFW Window
	*/
	GLFWwindow* GetInternal() 
	{ 
		return mWindow;
	}

	void GetSize(uint32_t& oWidth, uint32_t& oHeight)
	{
		oWidth = mWidth;
		oHeight = mHeight;
	}

	void SetSize(uint32_t iWidth, uint32_t iHeight)
	{
		mWidth = iWidth;
		mHeight = iHeight;
	}

private:
	// Width and Height in pixels
	uint32_t mWidth = 0;
	uint32_t mHeight = 0;

	std::string mWindowTitle;
	bool mFullscreen;
	GLFWwindow* mWindow;
};