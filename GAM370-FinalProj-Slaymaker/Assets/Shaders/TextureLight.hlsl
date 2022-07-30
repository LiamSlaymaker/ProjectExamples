#pragma pack_matrix(row_major)

// Material properties
struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;  // Hack: w holds the specular power
};

// Light properties
struct PhongADS
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
};

// Basic Phong Illumination computation
void PhongModel(
	Material mat,
	PhongADS lightint,
	float3 L,
	float3 normal,
	float3 DirToEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec
)
{
	// every light source adds to the ambient light total
	ambient = mat.Ambient * lightint.Ambient;

	float diffuseFactor = dot(L, normal);

	// Diffuse and specular contribution only if not facing away from light source
	if (diffuseFactor > 0.0f)
	{
		diffuse = diffuseFactor * mat.Diffuse * lightint.Diffuse;

		float3 r = reflect(-L, normal);		// Compute the reflected ray of light
		float spec_val = max(dot(r, DirToEye.xyz), 0);
		float specFactor = pow(spec_val, mat.Specular.w);
		spec = specFactor * mat.Specular * lightint.Specular;
	}
	else
	{
		diffuse = float4(0, 0, 0, 0);
		spec = float4(0, 0, 0, 0);
	}
}

// Directional Light Data
struct DirectionalLight
{
	PhongADS LightInt;
	float4 Direction;
};

// Point Light data
struct PointLight
{
	PhongADS LightInt;
	float4 Position;
	float4 Attenuation;
	float Range;
};

// Spot light data
struct SpotLight
{
	PhongADS LightInt;
	float4 Position;
	float4 Attenuation;
	float4 Direction;
	float SpotExp;
	float Range;
};

Texture2D mainTexture : register(t0);

SamplerState aSampler : register(s0);

//  Constant Buffers
cbuffer CamData : register(b0)
{
	float4x4 View;
	float4x4 Projection;
};

cbuffer LightParameters : register (b1)
{
	DirectionalLight DirLight;
	PointLight PntLight;
	PointLight PntLight2;
	PointLight PntLight3;
	SpotLight SpLight;
	float4 EyePosWorld;
};

cbuffer InstanceData : register(b2)
{
	float4x4 World;
	float4x4 WorldInv;
	Material Mater;
};

cbuffer FogData : register(b3)
{
	float FogStart;
	float FogRange;
	float Height;
	float4 FogColor;
	bool Set;
};

// Directional Light Computation
void ComputeDirectionalLight(
	Material mat,
	DirectionalLight DirLit,
	float4 normal,
	float4 DirToEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec
)
{
	// Zeroing out the initial ADS contributions
	ambient = float4(0, 0, 0, 0);
	diffuse = float4(0, 0, 0, 0);
	spec = float4(0, 0, 0, 0);

	// we compute ligth in the model-space 
	// For directional light, the source is infinitely far, so translation have no effect
	float3 L = normalize(mul(-DirLit.Direction.xyz, (float3x3)WorldInv));

	PhongModel(mat, DirLit.LightInt, L, normal.xyz, DirToEye.xyz, ambient, diffuse, spec);
}

// Point Light Computation
void ComputePointLight(
	Material mat,
	PointLight PLight,
	float4 posms,
	float4 normal,
	float4 DirToEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec
)
{
	// Zeroing out the ADS contributions
	ambient = float4(0, 0, 0, 0);
	diffuse = float4(0, 0, 0, 0);
	spec = float4(0, 0, 0, 0);

	// we compute the model-space position
	float3 litPosMS = mul(PLight.Position, WorldInv).xyz;
	float3 L = litPosMS - posms.xyz;

	// Early out if out of range
	float d = length(L);
	if (d > PLight.Range)
		return;

	L /= d;	// normalize 

	PhongModel(mat, PLight.LightInt, L, normal.xyz, DirToEye.xyz, ambient, diffuse, spec);

	// Now we attenuate based on range
	float att = 1 / dot(PLight.Attenuation.xyz, float3(1, d, d * d));
	// Ambient not attenuated
	diffuse *= att;
	spec *= att;
}

// Spot Light Computation
void ComputeSpotLight(
	Material mat,
	SpotLight SptLight,
	float4 posms,
	float4 normal,
	float4 DirToEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec
)
{
	// Zeroing out the ADS contributions
	ambient = float4(0, 0, 0, 0);
	diffuse = float4(0, 0, 0, 0);
	spec = float4(0, 0, 0, 0);

	// ****** Same calculations as Point Light  ****

	// we compute the model-space position
	float3 litPosMS = mul(SptLight.Position, WorldInv).xyz;
	float3 L = litPosMS - posms.xyz;

	float d = length(L);
	if (d > SptLight.Range)
		return;

	L /= d;  // normalize 

	PhongModel(mat, SptLight.LightInt, L, normal.xyz, DirToEye.xyz, ambient, diffuse, spec);

	// Now we attenuate based on range
	float att = 1 / dot(SptLight.Attenuation.xyz, float3(1, d, d * d));

	// Ambient not attenuated
	diffuse *= att;
	spec *= att;

	// *****************************************

	// now we compute the spotlight factor
	float4 msLitDir = SptLight.Direction;
	msLitDir.w = 0; // making sure we have a vector and not a point
	msLitDir = normalize(mul(msLitDir, WorldInv));
	float spot = pow(max(dot(-L, msLitDir.xyz), 0), SptLight.SpotExp);

	ambient *= spot;
	diffuse *= spot;
	spec *= spot;
}



// ******* Shader operations
struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 PosMS : POSITION;
	float4 Norm : NORMAL;
	float3 PosW: TEXCOORD0;
	float2 Tex : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.PosMS = input.Pos;  // We pass along the raw model space position 
	output.Norm = input.Norm;	 // and the face normal


	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = input.Tex;
	output.PosW = mul(input.Pos, World);

	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
	// Compute light values in model-space
	float4 msEyePos = mul(EyePosWorld, WorldInv);
	float4 msDirToEye = normalize(msEyePos - input.PosMS);

	float4 ambient = float4(0, 0, 0, 0);
	float4 diffuse = float4(0, 0, 0, 0);
	float4 spec = float4(0, 0, 0, 0);

	float4 A, D, S;

	ComputeDirectionalLight(Mater, DirLight, normalize(input.Norm), msDirToEye, A, D, S);

	ambient += A;
	diffuse += D;
	spec += S;

	ComputePointLight(Mater, PntLight, input.PosMS, normalize(input.Norm), msDirToEye, A, D, S);

	ambient += A;
	diffuse += D;
	spec += S;

	ComputePointLight(Mater, PntLight2, input.PosMS, normalize(input.Norm), msDirToEye, A, D, S);

	ambient += A;
	diffuse += D;
	spec += S;

	ComputePointLight(Mater, PntLight3, input.PosMS, normalize(input.Norm), msDirToEye, A, D, S);

	ambient += A;
	diffuse += D;
	spec += S;

	ComputeSpotLight(Mater, SpLight, input.PosMS, normalize(input.Norm), msDirToEye, A, D, S);

	ambient += A;
	diffuse += D;
	spec += S;


	//float4 litColor = ambient + diffuse + spec;

	float4 texColor = mainTexture.Sample(aSampler, input.Tex);

	float4 litColor = ambient + diffuse + spec;
	
	litColor = litColor * texColor;
	
	if (Set == true) {
		float3 P0 = float3(0, Height, 0);
		float3 n = float3(0, -1, 0);

		float3 P1 = input.PosW.xyz;
		float3 P2 = EyePosWorld.xyz;
		bool P1Above = (dot(P1 - P0, n) < 0);
		bool P2Above = (dot(P2 - P0, n) < 0);
		float t = dot(P0 - P1, n) / dot(P2 - P1, n);
		float3 P3 = P1 + t * (P2 - P1);
		float dist = 0;

		if (P1Above && !P2Above) {
			dist = length(P2 - P3);
		}
		else if (!P1Above && P2Above) {
			dist = length(P3 - P1);
		}
		else if (!P1Above && !P2Above) {
			dist = length(P1 - P2);
		}

		float FogPercent = saturate((dist - FogStart) / FogRange);
		litColor = litColor * (1 - FogPercent) + FogColor * FogPercent;
	}

	return litColor;
}
