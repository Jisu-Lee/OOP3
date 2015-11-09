matrix gLocalMatrix : Local;
matrix gWorldMatrix : World;
matrix gViewMatrix : View;
matrix gProjectionMatrix : Projection;
float4 gWorldLightPosition;
struct VS_INPUT
{
	float4 mPosition : POSITION;
	float3 Normal : NORMAL0;
	float2 mTexCoord : TEXCOORD0;

};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float3 mDiffuse : TEXCOORD1;
	float2 mTexCoord : TEXCOORD0;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;
	Output.mPosition = mul(Input.mPosition, gLocalMatrix);
	Output.mPosition = mul(Output.mPosition, gWorldMatrix);

	float3 lightDir = Output.mPosition.xyz - gWorldLightPosition.xyz;
	lightDir = normalize(lightDir);


	Output.mPosition = mul(Output.mPosition, gViewMatrix);
	Output.mPosition = mul(Output.mPosition, gProjectionMatrix);

	float3 worldNormal = mul(Input.Normal, (float3x3)gWorldMatrix);
	worldNormal = normalize(worldNormal);
	Output.mDiffuse = dot(-lightDir, worldNormal);

	Input.mPosition.w = 1.0f;
	Output.mDiffuse = Input.Normal;
	Output.mTexCoord = Input.mTexCoord;
	return Output;
}

texture DiffuseMap : TEX2D;
sampler DiffuseSampler = sampler_state
{
	Texture = (DiffuseMap);
	Filter = MIN_MAG_MIP_LINEAR;
	
};

struct PS_INPUT{
	float4 mPosition : POSITION;
	float3 mDiffuse : TEXCOORD1;
	float2 mTexCoord : TEXCOORD0;
};


float4 ps_main(PS_INPUT Input) : COLOR
{
	float3 diffuse = saturate(Input.mDiffuse);

	float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);
	return albedo;
	//return float4(0.0f, 0.0f, 0.0f, 1.0f);
}


//--------------------------------------------------------------//
// Technique Section for ColorShader
//--------------------------------------------------------------//
technique ColorShader
{
	pass Pass_1
	{
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}

}
