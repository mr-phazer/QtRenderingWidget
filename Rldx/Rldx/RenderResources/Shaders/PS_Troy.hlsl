//#include "..\troy_lib.hlsli"
#include "include\troy_lib2.hlsli"

///float4 ps30_main(in PixelInputType input) : COLOR

float3 doFakeSSR(in float3 env_specular, in float3 specular_color, in float smoothness);

float4 main(in PixelInputType input) : SV_TARGET
{
    
    //float4 diffuse_colour_DEBUG = shaderTextures[t_Diffuse].Sample(SamplerLinear, input.tex1.xy);
    //return float4(diffuse_colour_DEBUG.rgb, 1.0f);

    float3 eye_vector;
    float3 light_vector;
    float4 diffuse_colour;
    float4 specular_colour;
    float smoothness;
    float3x3 basis;
    float3 N;
    bool faction_colours = true;

	//	Get the inputs...
    ps30_get_shared_inputs(eye_vector, light_vector, diffuse_colour, specular_colour, smoothness, basis, N, input, faction_colours);

    
    //if (!PisTextureSRGB[t_Diffuse])
    //    diffuse_colour.rgb = _gamma(diffuse_colour.rgb);

	// Get calculated pixel normal (from normal map x TBN)
    float3 pixel_normal = getPixelNormal(input);    
    

    // read shadow map
    float2 shadow_buffer_UV;
    shadow_buffer_UV.xy = input.position.xy * screenRes.zw;

    float shadow = float4(Shadow_mapTexture.Sample(s_point, shadow_buffer_UV.xy).rgb, 1).r;

    //return float4(get_environment_colour(pixel_normal, 1.0f), 1);

    shadow = 1.0f; // TODO: disble shadows
    // View Space -> UV space, get AO, store AO in material
    //float2 screenUV = (input.screenPos.xy / input.screenPos.w) * 0.5f + 0.5f;
    //float AO = getSSAO(input);
    //float ambient_occlusion = 1.0;

    //if (use_AO)
    //{
    //    ambient_occlusion = AO;
    //}
    //else
        float ambient_occlusion = 1.0;

    //return float4(AO.xxx, 1); // TODO: REMOVE debugging code

	//  Create the standard material...
    R2_4_StandardLightingModelMaterial standard_mat = R2_4_create_standard_lighting_material(diffuse_colour.rgb, specular_colour.rgb, pixel_normal, smoothness,
    float4(input.Wpos.xyz, 1), shadow, ambient_occlusion);

    

    //return float4(1, 1, 0, 1);
    //return float4(diffuse_colour.rgb, 1);
	//  Light the pixel...
    
    
    


    float3 hdr_linear_col = standard_lighting_model_directional_light(

        lightData[0].radiance * lightData[0].color.rgb,
        -light_vector,
        eye_vector,
        standard_mat
    );

	//  Tone-map the pixel...
    float3 ldr_linear_col = (saturate(Uncharted2ToneMapping(1.3  * hdr_linear_col)));
    //float3 ldr_linear_col = saturate(tone_map_linear_hdr_pixel_value(hdr_linear_col));
//----------------------------------------------------------

	//  Return gamma corrected value...
    return float4( /*_gamma*/(ldr_linear_col), 1.0f);
    //return float4( /*_gamma*/(hdr_linear_col), 1.0f);
}

float3 doFakeSSR(in float3 env_specular, in float3 specular_color, in float smoothness)
{

    //float3 reflectivity = get_reflectivity_env_light_material(light_vector, pixel_normal, eye_vector, 1.f - smoothness, specular_colour);

    //float3 base_specular_env = 0;

    //float rough = (1 - smoothness);

    //rough = pow(rough, 6);
    //float smooth_pow = saturate(pow(smoothness, 4));

    //float3 env_color = get_environment_colour(pixel_normal, (rough * 6) + 1);

    //float3 amb_color = cube_ambient(pixel_normal);

    //float3 color = lerp(0, 4 * specular_colour.rgb * env_color, smooth_pow);

    //return float4(smooth_pow.xxx, 1);;
    //return float4(saturate(lumaBasedReinhardToneMapping(color)), 1);;
    //----------------------------------------------------------

    return float3(1, 1, 1);
}