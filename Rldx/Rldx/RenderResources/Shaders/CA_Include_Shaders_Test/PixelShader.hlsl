#include "standard_lighting_assembly_kit.hlsli"



float4 main() : SV_TARGET
{
    VS_OUTPUT pixelData;
    pixelData.Position = float4(0, 0, 0, 0);
    pixelData.Btgt = float3(0, 0, 0);
    pixelData.Nml = float3(0, 0, 0);
    pixelData.Tgt = float3(0, 0, 0);
    pixelData.Wpos = float3(0, 0, 0);
    pixelData.Color = float4(0, 0, 0,0);
    pixelData.TexCoord = float4(0, 0,0,0);
    pixelData.I = float3(0, 0,0);

    
    
    //return ps30_main_UPDATED(pixelData);

	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}