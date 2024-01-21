#version 460

in vec2 vTexCoord0;

out vec4 FragColor;

uniform sampler2D uTexture0;

void main()
{
	FragColor = texture2D(uTexture0, vTexCoord0);
}