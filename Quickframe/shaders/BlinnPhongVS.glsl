#version 460 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Normal;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec2 TexCoord0;


uniform mat4 uMVP;
uniform mat4 uLightMVP;
uniform mat3 uNormalMatrix;
uniform mat4 uWorld;

out vec2 vTexCoord0;
out vec3 vNormal;
out vec3 vWorldPos;
out vec4 vLightSpacePos;
out mat3 vTBN;

void main()
{
	gl_Position = uMVP * vec4(Pos, 1.0);

	vec3 T = normalize(vec3(uWorld * vec4(Tangent, 0.0)));
	vNormal = uNormalMatrix * Normal;
	vec3 N = vNormal;

	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	vTBN = mat3(T, B, N);

	vTexCoord0 = TexCoord0;

	vec4 WorldPos = uWorld * vec4(Pos, 1.0);
	vWorldPos = vec3(WorldPos);

	vLightSpacePos = uLightMVP * vec4(Pos, 1.0);
}