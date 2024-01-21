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

#define COLOR_TEXTURE_UNIT GL_TEXTURE0
#define COLOR_TEXTURE_UNIFORM_IDX 0
#define COLOR_TEXTURE_UNIFORM "uColorTex"

#define NORMAL_TEXTURE_UNIT GL_TEXTURE1
#define NORMAL_TEXTURE_UNIFORM_IDX 1
#define NORMAL_TEXTURE_UNIFORM "uNormalTex"

#define SPECULAR_EXPONENT_TEXTURE_UNIT GL_TEXTURE2
#define SPECULAR_EXPONENT_TEXTURE_UNIFORM_IDX 2
#define SPECULAR_EXPONENT_TEXTURE_UNIFORM "uSpecularExponentTex"

#define SHADOW_MAP_0_TEXTURE_UNIT GL_TEXTURE3
#define SHADOW_MAP_0_TEXTURE_UNIFORM_IDX 3
#define SHADOW_MAP_0_TEXTURE_UNIFORM "uShadowMap0"

#define CUBEMAP_0_TEXTURE_UNIT GL_TEXTURE4
#define CUBEMAP_0_TEXTURE_UNIFORM_IDX 4
#define CUBEMAP_0_TEXTURE_UNIFORM "uCubeMap0"