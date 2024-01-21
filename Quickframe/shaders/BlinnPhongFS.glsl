#version 460

const int MAX_POINT_LIGHTS  = 4;
const int MAX_SPOT_LIGHTS  = 4;

in vec2 vTexCoord0;
in vec3 vNormal;
in vec3 vWorldPos;
in vec4 vLightSpacePos;
in mat3 vTBN;

out vec4 FragColor;

struct DirLight
{
	float Intensity;
	vec3 Color;
	vec3 Dir;
	float AmbientIntensity;
};

struct Attenuation
{
 float Constant;
 float Linear;
 float Exp;
};

struct PointLight
{
	float Intensity;
	vec3 Color;
	vec3 Position;
	Attenuation Atten;
};

struct SpotLight
{
	PointLight PL;
	vec3 Dir;
	float Cutoff;
};

struct Material
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float SpecularExponent;
};

uniform bool uColorTexAvailable;
uniform bool uNormalTexAvailable;
uniform bool uSpecularTexAvailable;
uniform bool uShadowEnabled;

uniform vec3 uCameraWorldPos;

uniform DirLight uDirLight;
uniform PointLight uPointLight[MAX_POINT_LIGHTS];
uniform SpotLight uSpotLight[MAX_SPOT_LIGHTS];
uniform int uDirLightNum;
uniform int uPointLightNum;
uniform int uSpotLightNum;

uniform Material uMaterial;

uniform sampler2D uColorTex;
uniform sampler2D uSpecularExponentTex;
uniform sampler2D uNormalTex;
uniform sampler2D uShadowMap0;

// Function Definitions for Directional, Point and Spot Lights
vec4 DirectionalLightContribution(DirLight iDirLight, vec3 iNormal);
vec4 PointLightContribution(PointLight iPointLight, vec3 iNormal);
vec4 SpotLightContribution(SpotLight iSpotLight, vec3 iNormal);
vec4 LightFunc(vec3 iColor, float iAmbientIntensity, vec3 iLightDir, vec3 iNormal);

// Shadow Function
float ShadowFactor();

void main()
{
	vec3 wNormal = vec3(0.f, 0.f, 0.f);
	if(uNormalTexAvailable)
	{
		wNormal = texture(uNormalTex, vTexCoord0).rgb;
		wNormal = normalize(wNormal * 2.0 - 1.0);
		wNormal = normalize(vTBN * wNormal);
	}
	else
	{
		wNormal = normalize(vNormal);
	}
	
	vec4 DirLightColor= vec4(0.f, 0.f, 0.f, 0.f);
	if(uDirLightNum > 0)
	{
		DirLightColor += DirectionalLightContribution(uDirLight, wNormal);
		DirLightColor *= uDirLight.Intensity;
		clamp(DirLightColor, 0.0, 1.0);
	}
	
	vec4 PointLightColor = vec4(0.f, 0.f, 0.f, 0.f);

	for(int i=0; i < uPointLightNum; i++)
	{
		PointLightColor += PointLightContribution(uPointLight[i], wNormal);
		PointLightColor *= uPointLight[i].Intensity;
		clamp(PointLightColor, 0.0, 1.0);
	}

	vec4 SpotLightColor = vec4(0.f, 0.f, 0.f, 0.f);

	for(int i=0; i < uSpotLightNum; i++)
	{
		SpotLightColor += SpotLightContribution(uSpotLight[i], wNormal);
		SpotLightColor *= uSpotLight[i].PL.Intensity;
		clamp(SpotLightColor, 0.0, 1.0);

	}

	vec4 LightColor = clamp(DirLightColor + PointLightColor + SpotLightColor, 0.f, 1.f);
	
	if(uShadowEnabled)
	{
		LightColor *= ShadowFactor();
	}

	vec4 ColorTex = vec4(1.f, 1.f, 1.f, 1.f);
	if(uColorTexAvailable)
	{
		ColorTex = texture2D(uColorTex, vTexCoord0);
	}

	FragColor = ColorTex * LightColor;
}

vec4 DirectionalLightContribution(DirLight iDirLight, vec3 iNormal)
{
	return LightFunc(iDirLight.Color, iDirLight.AmbientIntensity, iDirLight.Dir, iNormal);
}

vec4 PointLightContribution(PointLight iPointLight, vec3 iNormal)
{
	vec3 LightDir = vWorldPos - iPointLight.Position;
	float Distance = length(LightDir);
	LightDir = normalize(LightDir);
	vec4 Result = LightFunc(iPointLight.Color, 0.f, LightDir, iNormal);

	float Attenuation = iPointLight.Atten.Constant +
						iPointLight.Atten.Linear * Distance +
						iPointLight.Atten.Exp * Distance * Distance;

	return Result / Attenuation;
}

vec4 SpotLightContribution(SpotLight iSpotLight, vec3 iNormal)
{
	vec3 LightToPixel = normalize(vWorldPos - iSpotLight.PL.Position);
	float SpotFactor = dot(LightToPixel, iSpotLight.Dir);

	if(SpotFactor > iSpotLight.Cutoff)
	{
		vec4 Color = PointLightContribution(iSpotLight.PL, iNormal);
		SpotFactor = 1.f - ((1.f - SpotFactor) / (1.f - iSpotLight.Cutoff));
		return Color * SpotFactor;
	}

	return vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

vec4 LightFunc(vec3 iColor, float iAmbientIntensity, vec3 iLightDir, vec3 iNormal)
{
	// Ambient Contribution
	vec4 Ambient = vec4(uMaterial.Ambient, 1.0f) *
						vec4(iColor, 1.0f) *
						iAmbientIntensity;
    clamp(Ambient, 0.f, 1.f);

	// Diffuse Contribution
	float DiffuseFactor = dot(normalize(iNormal), -iLightDir);
	
	vec4 Diffuse = vec4(0,0,0,0);
	if(DiffuseFactor > 0 )
	{
		Diffuse = vec4(iColor, 1.0f) *
						vec4(uMaterial.Diffuse, 1.0f) *
						DiffuseFactor;
		clamp(Diffuse, 0.f, 1.f);
	}

	// Specular Contribution
	vec4 Specular = vec4(0.f, 0.f, 0.f, 0.f);
	vec3 PixelToCamera = normalize(uCameraWorldPos - vWorldPos);
	vec3 Halfway = normalize(-iLightDir + PixelToCamera);
	float SpecularFactor = dot(iNormal, Halfway);

	if(SpecularFactor > 0)
	{
		float SpecularExponent = uMaterial.SpecularExponent;
		if(uSpecularTexAvailable)
		{
			SpecularExponent = texture2D(uSpecularExponentTex, vTexCoord0).r * 255.0;
		}

		SpecularFactor = pow(SpecularFactor, SpecularExponent);
		Specular = vec4(iColor, 1.0f) *
						vec4(uMaterial.Specular, 1.0f) *
						SpecularFactor;

		clamp(Specular, 0.f, 1.f);
	}

	return Ambient + Diffuse + Specular;
}

float ShadowFactor()
{
	vec3 ProjCoord = vLightSpacePos.xyz / vLightSpacePos.w; // Get [-1, 1] coords
	vec2 UVs;
	UVs.x = 0.5 * ProjCoord.x + 0.5;
	UVs.y = 0.5 * ProjCoord.y + 0.5;
	float z = 0.5 * ProjCoord.z + 0.5;
	float Depth = texture2D(uShadowMap0, UVs).x;

	float Bias = 0.001;

	if(Depth + Bias < z)
	{
		return 0.5;
	}

	else
	{
		return 1.0;
	}
}