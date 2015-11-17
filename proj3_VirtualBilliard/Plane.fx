matrix gWorldMatrix : World;
matrix gWorldViewProjection;

float4 gWorldCameraPosition;
float4 gWorldLightPosition;
float3 gLightColor;




struct VS_INPUT
{
	float4 mPosition : POSITION;
	float3 Normal : NORMAL0;
	float3 mTangent : TANGENT;
	float3 mBinormal : BINORMAL;
	float2 mTexCoord : TEXCOORD0;

};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mUV : TEXCOORD0;
	float3 mLightDir : TEXCOORD1;
	float3 mViewDir : TEXCOORD2;
	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;

};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;
	Output.mPosition = mul(Input.mPosition, gWorldViewProjection);
	Output.mUV = Input.mTexCoord;

	float4 worldPosition = mul(Input.mPosition, gWorldMatrix);
	float3 lightDir = Output.mPosition.xyz - gWorldLightPosition.xyz;
	Output.mLightDir = normalize(lightDir);

	float3 viewDir = normalize(Output.mPosition.xyz - gWorldCameraPosition.xyz);

	Output.mViewDir = viewDir;

	float3 worldNormal = mul(Input.Normal, (float3x3)gWorldMatrix);
	Output.N = normalize(worldNormal);


	float3 worldTangent = mul(Input.mTangent, (float3x3)gWorldMatrix);
		Output.T = normalize(worldTangent);

	float3 worldBinormal = mul(Input.mBinormal, (float3x3)gWorldMatrix);
		Output.B = normalize(worldBinormal);


	return Output;
}

texture DiffuseMap : TEX2D;
sampler DiffuseSampler = sampler_state
{
	Texture = (DiffuseMap);
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;

};

texture NormalMap : TEX2D;
sampler NormalSampler = sampler_state
{
	Texture = (NormalMap);
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;
};
texture SpecularMap : TEX2D;
sampler SpecularSampler = sampler_state
{
	Texture = (SpecularMap);
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;
};

struct PS_INPUT{
	float2 mTexCoord : TEXCOORD0;
	float3 mLightDir : TEXCOORD1;
	float3 mViewDir : TEXCOORD2;
	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;
};

float4 ps_main(PS_INPUT Input) : COLOR
{

	float3 tangentNormal = tex2D(NormalSampler, Input.mTexCoord).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(Input.T), normalize(Input.B), normalize(Input.N));
		TBN = transpose(TBN);

	float3 worldNormal = mul(TBN, tangentNormal);

		float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);
		float3 lightDir = normalize(Input.mLightDir);
		float3 diffuse = saturate(dot(worldNormal, -lightDir));
		diffuse = gLightColor * albedo.rgb * diffuse;
	
	float3 specular = 0;
	if (diffuse.x > 0){
		float3 reflection = reflect(lightDir, worldNormal);
			float3 viewDir = normalize(Input.mViewDir);

			specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 0.0f);

		float4 specularIntensity = tex2D(SpecularSampler, Input.mTexCoord);
			specular *= specularIntensity.rgb * gLightColor;
	}

	float3 ambient = float3(0.7f, 0.7f, 0.7f) * albedo;

		return float4(ambient + diffuse + specular, 1);


	
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
