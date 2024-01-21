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

#include "Input.h"

Input* Input::mInput = nullptr;

Input* Input::GetInstance()
{
	if (mInput == nullptr)
	{
		mInput = new Input;
	}
	return mInput;
}

void Input::Update()
{
	if (mHidingCursor)
	{
		mDeltaX = 0;
		mDeltaY = 0;
		mOldX = mCurrentX;
		mOldY = mCurrentY;
		mHidingCursor = false;
	}
}

void Input::Reset()
{
	for (int i = 0; i < GLFW_KEY_LAST; i++)
	{
		mKeyReleased[i] = false;
	}

	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
	{
		mMouseReleased[i] = false;
	}

	mDeltaX = 0.0;
	mDeltaY = 0.0;
	mIsCursorMoved = false;
}

void Input::SetCursorPos(double iXPos, double iYPos)
{
	mIsCursorMoved = true;

	mCurrentX = iXPos;
	mCurrentY = iYPos;

	if (mFirstMove)
	{
		mOldX = mCurrentX;
		mOldY = mCurrentY;
		mFirstMove = false;
	}

	mDeltaX = mCurrentX - mOldX;
	mDeltaY = mOldY - mCurrentY;

	mOldX = mCurrentX;
	mOldY = mCurrentY;
}

void Input::GetMouseCursorInfo(MouseCursorInfo& oMouseCursorInfo)
{
	oMouseCursorInfo.CursorPosX = mCurrentX;
	oMouseCursorInfo.CursorPosY = mCurrentY;
	oMouseCursorInfo.DeltaX = mDeltaX;
	oMouseCursorInfo.DeltaY = mDeltaY;
	oMouseCursorInfo.isCursorMoved = mIsCursorMoved;
}

