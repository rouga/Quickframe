#version 460

layout (location = 0) in vec3 vPos;

uniform mat4 uMVP;

void main()
{
	gl_Position = uMVP * vec4(vPos, 1.0);
}

