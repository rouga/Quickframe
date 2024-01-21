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

#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <exception>
#include <memory>
#include <spdlog/spdlog.h>

#include "Window.h"
#include "Renderer.h"
#include "Entity.h"
#include "Camera.h"
#include "SceneManager.h"

// TODO FORWARD DECLARATION


/**
 * Singleton Class managing all other subsystems
 */
class Engine
{
public:

	Engine(Engine& iOther) = delete;
	void operator=(const Engine&) = delete;

	static Engine* GetInstance();

	~Engine();

	/**
	 * Run the Engine's loop
	 */
	void Run();

	/**
	 * @brief Initialize all the engines subsystems
	*/
	void Initialize();

	/**
	 * @brief : Check if OpenGL functions Loader is properly initialized
	 * @return : true if OGL loader is initialized
	*/
	static bool isGLfuncLoaded() { return mGLFuncLoaded; }

	// Input Handling

	void OnCursorPos(double iX, double iY);

	void OnMouseButton(int iButton, int iAction, int iMods);

	void OnKeyboard(int iKey, int iScancode, int iAction, int iMods);

	void OnResize(int iWidth, int iHeight);

	// Getters
	Window* GetWindow() const { return mWindow.get(); }

protected:
	Engine();

	static Engine* mApp;
	
private:
	/**
	 * @brief : Create a window and OGL context
	 * @param iWindowInfo : Struct defining the window properties
	*/
	void CreateWnd(Window::WindowInfo iWindowInfo);
	void InitGLFWCallbacks();
	void InitSceneManager();
	void PrintAdapterInfo();

	static bool mGLFuncLoaded;
	double mDeltaTime;
	std::unique_ptr<Window> mWindow;
	std::unique_ptr<Renderer> mRenderer;
	std::unique_ptr<SceneManager> mSceneManager;
};