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

#include <GLFW/glfw3.h>

/**
 * @brief : Input class saves the Keyboard/Mouse states for the current frame. 
 * They are set by GLFW Callbacks
*/
class Input
{
public:
	struct MouseCursorInfo
	{
		double DeltaX = 0.0;
		double DeltaY = 0.0;
		double CursorPosX = 0.0;
		double CursorPosY = 0.0;
		bool isCursorMoved = false;
	};

	Input(Input& iOther) = delete;
	void operator=(const Input&) = delete;

	static Input* GetInstance();

	/**
	* @brief Update is called after the Keyboard and Mouse states are set (after GLFW Polling)
	* Currently it is used to avoid big cursor deltas when the cursor state changes from normal to Disabled
	*/
	void Update();

	/**
	 * @brief Reset method resets Keyboard/Mouse release state to false 
	 * Released state can persist for one frame, Unlike Pressed which can be held for multiple frames
	*/
	void Reset();

	void SetKeyPressed(int iKey)
	{
		mKeyPressed[iKey] = true;
	}

	void SetKeyReleased(int iKey)
	{
		mKeyReleased[iKey] = true;
		mKeyPressed[iKey] = false;
	}

	void SetMousePressed(int iButton)
	{
		if (iButton == GLFW_MOUSE_BUTTON_RIGHT && !mMousePressed[iButton])
		{
			mHidingCursor = true;
		}
		mMousePressed[iButton] = true;
	}

	void SetMouseReleased(int iButton)
	{
		mMouseReleased[iButton] = true;
		mMousePressed[iButton] = false;

		if (iButton == GLFW_MOUSE_BUTTON_RIGHT)
		{
			mHidingCursor = false;
		}
	}

	void SetCursorPos(double xPos, double yPos);

	bool IsKeyPressed(int iKey) const { return mKeyPressed[iKey]; }
	bool IsKeyReleased(int iKey) const { return mKeyReleased[iKey]; }
	bool IsMousePressed(int iButton) const { return mMousePressed[iButton]; }
	bool IsMouseReleased(int iButton) const { return mMouseReleased[iButton]; }
	void GetMouseCursorInfo(MouseCursorInfo& oMouseCursorInfo);

protected:
	Input() {}

	static Input* mInput;

private:
	bool mKeyPressed[GLFW_KEY_LAST] = { false };
	bool mKeyReleased[GLFW_KEY_LAST] = { false };
	bool mMousePressed[GLFW_MOUSE_BUTTON_LAST] = { false };
	bool mMouseReleased[GLFW_MOUSE_BUTTON_LAST] = { false };

	double mOldX = 0.0;
	double mOldY = 0.0;
	double mCurrentX = 0.0;
	double mCurrentY = 0.0;
	double mDeltaX = 0.0;
	double mDeltaY = 0.0;

	bool mFirstMove = true;
	bool mIsCursorMoved = false;
	bool mHidingCursor = false;
};