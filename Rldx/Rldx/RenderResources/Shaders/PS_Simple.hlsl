//#include "..\layout.hlsli"
//#include "..\textures.hlsli"
//#include "..\tone_mapping.hlsli"
#include "include\attila_lib.hlsli"

float4 main(in PixelInputType input) : SV_TARGET0
{                                       
    return float4(input.color.rgb, 1);
}