#version 460

layout (location = 0) in vec3 Pos;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * vec4(Pos, 1);
}