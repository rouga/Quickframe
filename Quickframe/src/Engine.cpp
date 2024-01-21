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

#include "Engine.h"
#include "Input.h"
#include "Transform.h"
#include "Light.h"

Engine* Engine::mApp = nullptr;
bool Engine::mGLFuncLoaded = false;

Engine* Engine::GetInstance()
{
	if (mApp == nullptr) 
	{
		mApp = new Engine;
	}
	return mApp;
}

Engine::Engine()
{
}

Engine::~Engine()
{
	glfwTerminate();
}

void Engine::Run()
{
	static double wCurrentTick = 0.0;
	static double wLastFrameTick = 0.0;

	while (!glfwWindowShouldClose(mWindow->GetInternal()))
	{
		wCurrentTick = glfwGetTime();
		mDeltaTime = wCurrentTick - wLastFrameTick;
		wLastFrameTick = wCurrentTick; 
		// Reset Release state before calling input callbacks
		Input::GetInstance()->Reset();

		glfwPollEvents();
		Input::GetInstance()->Update();
		mSceneManager->GetActiveScene()->Update(mDeltaTime);

		mRenderer->Render(mSceneManager->GetActiveScene());

		// glfw: swap buffers
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(mWindow->GetInternal());
	}
}

void Engine::Initialize()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window::WindowInfo wWndInfo{};
	wWndInfo.mWidth = 1920;
	wWndInfo.mHeight = 1080;
	wWndInfo.mTitle = "Quickframe";
	wWndInfo.mFullscreen = false;

	CreateWnd(wWndInfo);

	glfwMakeContextCurrent(mWindow->GetInternal());

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to initialize GLAD");
	}
	PrintAdapterInfo();

	InitGLFWCallbacks();

	mRenderer = std::make_unique<Renderer>();
	mRenderer->Initialize();

	InitSceneManager();
}

void Engine::InitSceneManager()
{
	mSceneManager = std::make_unique<SceneManager>();
}

void Engine::PrintAdapterInfo()
{
	const GLubyte* vendor = glGetString(GL_VENDOR); // Returns the vendor
	const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	int mTextureUnits = 0;
	int mIBSize = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &mTextureUnits);
	glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &mIBSize);
	spdlog::info("VENDOR : {0:s}", (char*)vendor);
	spdlog::info("RENDERER : {0:s}", (char*)renderer);
	spdlog::info("OPENGL VERSION : {0:s}", (char*)glslVersion);
	spdlog::info("MAX TEXTURE IMAGE UNITS : {0:d}", mTextureUnits);
	spdlog::info("MAX INDEX COUNT : {0:d}", mIBSize);
}

void Engine::OnCursorPos(double iX, double iY)
{
	Input::GetInstance()->SetCursorPos(iX, iY);
}

void Engine::OnMouseButton(int iButton, int iAction, int iMods)
{
	if (iAction == GLFW_PRESS)
	{
		Input::GetInstance()->SetMousePressed(iButton);
	}
	else if (iAction == GLFW_RELEASE)
	{
		Input::GetInstance()->SetMouseReleased(iButton);
	}

	if (iButton == GLFW_MOUSE_BUTTON_RIGHT && iAction == GLFW_PRESS)
	{
		glfwSetInputMode(mWindow->GetInternal(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else if (iButton == GLFW_MOUSE_BUTTON_RIGHT && iAction == GLFW_RELEASE)
	{
		glfwSetInputMode(mWindow->GetInternal(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Engine::OnKeyboard(int iKey, int iScancode, int iAction, int iMods)
{
	if (iKey == GLFW_KEY_ESCAPE && iAction == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(mWindow->GetInternal(), GLFW_TRUE);
	}
	if (iAction == GLFW_PRESS)
	{
		Input::GetInstance()->SetKeyPressed(iKey);
	}
	else if (iAction == GLFW_RELEASE)
	{
		Input::GetInstance()->SetKeyReleased(iKey);
	}
}

void Engine::OnResize(int iWidth, int iHeight)
{
	mWindow->SetSize(iWidth, iHeight);
	glViewport(0, 0, iWidth, iHeight);
	float wAR = (float)iWidth / (float)iHeight;

	// Handle window minimization
	if (!std::isnan(wAR))
	{
		mSceneManager->GetActiveScene()->GetCamera()->SetAspectRatio(wAR);
	}
}

void Engine::CreateWnd(Window::WindowInfo iWindowInfo)
{
	mWindow = std::make_unique<Window>(iWindowInfo);
}

void Engine::InitGLFWCallbacks()
{
	glfwSetWindowUserPointer(mWindow->GetInternal(), (void*)this);

	glfwSetCursorPosCallback(mWindow->GetInternal(), [](GLFWwindow* iWindow, double iX, double iY)
		{
			Engine* App = static_cast<Engine*>(glfwGetWindowUserPointer(iWindow));
			App->OnCursorPos(iX, iY);
		}
	);

	glfwSetMouseButtonCallback(mWindow->GetInternal(), [](GLFWwindow* iWindow, int iButton, int iAction, int iMods)
		{
			Engine* App = static_cast<Engine*>(glfwGetWindowUserPointer(iWindow));
			App->OnMouseButton(iButton, iAction, iMods);
		}
	);

	glfwSetKeyCallback(mWindow->GetInternal(), [](GLFWwindow* iWindow, int iKey, int iScancode, int iAction, int iMods)
		{
			Engine* App = static_cast<Engine*>(glfwGetWindowUserPointer(iWindow));
			App->OnKeyboard(iKey, iScancode, iAction, iMods);
		}
	);

	glfwSetFramebufferSizeCallback(mWindow->GetInternal(), [](GLFWwindow* iWindow, int iWidth, int iHeight)
		{
			Engine* App = static_cast<Engine*>(glfwGetWindowUserPointer(iWindow));
			App->OnResize(iWidth, iHeight);
		}
	);
}