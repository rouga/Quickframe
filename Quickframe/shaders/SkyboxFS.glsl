#version 460 core

out vec4 FragColor;

in vec3 vTexCoords;

uniform samplerCube uCubeMap0;

void main()
{    
    FragColor = texture(uCubeMap0, vTexCoords);
}