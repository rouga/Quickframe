#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 vTexCoords;

uniform mat4 uVP;

void main()
{
    vTexCoords = aPos;
    gl_Position = (uVP * vec4(aPos, 1.0)).xyww;
}  