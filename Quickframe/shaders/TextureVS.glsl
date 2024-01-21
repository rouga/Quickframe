#version 460

layout (location = 0) in vec3 Pos;
layout (location = 3) in vec2 TexCoord0;

uniform mat4 uMVP;

out vec2 vTexCoord0;

void main()
{
	gl_Position = uMVP * vec4(Pos, 1.0);
	vTexCoord0 = TexCoord0;
}