 //#pragma once                                           

#include "layout.hlsli"
#include "textures.hlsli"
#include "PS_ConstBuffers.hlsli"
#include "tone_mapping.hlsli"

float getShadow(in PixelInputType input)
{
    float2 shadow_buffer_UV;
    shadow_buffer_UV.xy = input.position.xy * screenRes.zw;
    float shadow = float4(Shadow_mapTexture.Sample(s_point, shadow_buffer_UV.xy).rgb, 1).r;

    return shadow;
}                                            

float getSSAO(PixelInputType input)
{
    //float2 screenUV = (input.screenPos.xy / input.screenPos.w) * 0.5f + 0.5f;
    //return ao_mapTexture.SampleLevel(SamplerLinear, screenUV, 0);

    float2 screenUV;
    screenUV.x = 0.5f + (input.screenPos.x / input.screenPos.w * 0.5f);
    screenUV.y = 0.5f - (input.screenPos.y / input.screenPos.w * 0.5f);

    //float2 screenUV = (input.screenPos.xy / input.screenPos.w) * 0.5f + 0.5f;
    return ao_mapTexture.SampleLevel(SamplerLinear, screenUV, 0).r;
}

float getAO_old(in float2 screenUV)
{
    //float2 screenUV = (input.screenPos.xy / input.screenPos.w) * 0.5f + 0.5f;
    return ao_mapTexture.SampleLevel(SamplerLinear, screenUV, 0).r;
}

float3 getBlueFromOrangeNormalMap(float4 _orangeMapSample)
{
    float3 BlueSample = 0;
    BlueSample.y = _orangeMapSample.r * _orangeMapSample.a;
    BlueSample.x = /*1.0 - */_orangeMapSample.g;
    BlueSample = (BlueSample * 2.0f) - 1.0f;

    BlueSample.z = sqrt(1 - BlueSample.x * BlueSample.x - BlueSample.y * BlueSample.y);

    return BlueSample;
}

float3 getPixelNormal(PixelInputType input)
{
    //float3x3 basis = float3x3(normalize(input.tangent), normalize(input.normal), normalize(input.binormal));
    float3x3 basis = float3x3(normalize(input.tangent.xyz), normalize(input.binormal.xyz), normalize(input.normal.xyz));
    float4 NormalTex = shaderTextures[t_Normal].Sample(s_anisotropic, input.tex1);

    float3 Np = 0;
    Np.x = NormalTex.r * NormalTex.a;
    Np.y = /*1.0 - */NormalTex.g;
    Np = (Np * 2.0f) - 1.0f;

    Np.z = sqrt(1 - Np.x * Np.x - Np.y * Np.y);

    //Np.y = -Np.y;
    //return normalize(mul(normalize(Np.xyz), basis));
    return normalize(mul(normalize(Np), basis));
}