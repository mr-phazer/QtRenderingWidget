//#include "..\layout.hlsli"
//#include "..\textures.hlsli"
//#include "..\tone_mapping.hlsli"
#include "include\attila_lib.hlsli"

float4 main(in PixelInputType input) : SV_TARGET0
{
    float4 position = input.position;
    //float2 tex = input.tex1;

    //float3 normal = input.normal;
    //float3 tangent = input.tangent;
    //float3 binormal = input.binormal;
    //float ssao = getSSAO(input);

    //return float4(ssao.xxx, 1.0f);
    
        
    return float4(input.color);

    //float3 pxieLnormal = getPixelNormal(input);;

    //return float4(cube_ambient(pxieLnormal), 1);

}