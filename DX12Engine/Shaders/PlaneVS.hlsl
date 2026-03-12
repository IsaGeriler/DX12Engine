cbuffer staticMeshBuffer : register(b0) {
	float4x4 W;   // 4x4 World Matrix
	float4x4 VP;  // 4x4 View-Projection Matrix
};

struct VS_INPUT {
	float4 Pos : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float2 TexCoords : TEXCOORD;
};

struct PS_INPUT {
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float2 TexCoords : TEXCOORD;
};

PS_INPUT VS(VS_INPUT input) {
	PS_INPUT output;

	// Apply Transformations in Shader
	output.Pos = mul(input.Pos, W);
	output.Pos = mul(output.Pos, VP);
	output.Normal = mul(input.Normal, W);
	output.Tangent = mul(input.Tangent, W);
	output.TexCoords = input.TexCoords;

	return output;
}