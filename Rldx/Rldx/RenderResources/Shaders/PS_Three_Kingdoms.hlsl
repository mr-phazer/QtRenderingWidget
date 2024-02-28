#include "include\TW3K_LIB.hlsli"

//float3 getPixelNormal(PixelInputType input)
//{
    
//    float3x3 basis = float3x3(normalize(input.tangent.xyz), normalize(input.binormal.xyz), normalize(input.normal.xyz)); // works in own shader

//    float4 NormalTex = shaderTextures[t_Normal].Sample(s_anisotropic, input.tex1);

//    float3 Np = 0;
//    Np.x = NormalTex.r * NormalTex.a;
//    Np.y = 1.0 - NormalTex.g;
//    Np = (Np * 2.0f) - 1.0f;

//    Np.z = sqrt(1 - Np.x * Np.x - Np.y * Np.y);

//    //Np.y = -Np.y;
//    return normalize(mul(normalize(Np.xyz), basis));
//}

float4 main(in PixelInputType input) : SV_TARGET
{
    //return float4(env_radiance, 0, 0, 1);                                                   

    //PIXELDATA OUT;

    float3 eye_vector;
    float3 light_vector;
    float4 base_colour;
    float3 normal;
    float roughness;
    float metalness;
    float occlusion;
    float3x3 basis;
    bool faction_colours = true;

	//	Get the inputs...
    ps30_get_shared_inputs(eye_vector, light_vector, base_colour, /*normal,*/roughness, metalness, occlusion, /*basis, */input, faction_colours);     
    
    
	//moving to allow blending
    //float3 pixel_normal = normalize(mul(normalize(normal), basis));
    float3 pixel_normal = getPixelNormal(input);

    //  Create the standard material.  This is what gets written to the gbuffer...
    R2_5_StandardLightingModelMaterial_For_GBuffer standard_mat_compressed = R2_5_create_standard_lighting_material_for_gbuffer(base_colour.rgb, pixel_normal, roughness, metalness, occlusion);

	//	Create the uncompressed material.  This is what is read from the gbuffer...
    R2_5_StandardLightingModelMaterial_For_Lighting slm_uncompressed = R2_5_get_slm_for_lighting(standard_mat_compressed);

	//	Apply faction colours...
    //apply_faction_colours(slm_uncompressed.Diffuse_Colour.rgb, input.tex1.xy, faction_colours);

    //  Light the pixel...
    float3 hdr_linear_col = standard_lighting_model_directional_light(get_sun_colour(), light_vector, eye_vector, slm_uncompressed);

    ////  Tone-map the pixel...
    float3 ldr_linear_col = saturate(tone_map_linear_hdr_to_linear_ldr(hdr_linear_col, Tone_Map_Black, Tone_Map_White));

    ////  Return gamma corrected value...
    //OUT.Colour = float4(_gamma(ldr_linear_col), 1.0f);

    return float4(ldr_linear_col, 1);
    //return float4(hdr_linear_col, 1);
}

#if 0
float4 ___main(in PixelInputType input) : SV_TARGET
{

    //PIXELDATA OUT;

    float3 eye_vector = -normalize(input.viewDirection);
    float3 light_vector = -normalize(LightData[0].lightDirection);

    float4 base_colour;
    float3 normal = input.normal.xyz;
    float roughness;
    float metalness;
    float occlusion;
    float3x3 basis;
    bool faction_colours = true;

    //PS_OUTPUT output;

	// sample textures
    //float4 specular_colour = shaderTextures[t_BaseColor].Sample(s_anisotropic, input.tex1);
    //float4 diffuse_colour = shaderTextures[t_Diffuse].Sample(s_anisotropic, input.tex1);
    base_colour = shaderTextures[t_BaseColor].Sample(s_anisotropic, input.tex1).rgba;

    float4 tex_material_map = shaderTextures[t_MaterialMap].Sample(s_anisotropic, input.tex1);
    metalness = tex_material_map.r;
    roughness = tex_material_map.g * smoothness_curve;

    float4 MaskTex = shaderTextures[t_Mask].Sample(s_anisotropic, input.tex1);
    float4 NormalMapTex = shaderTextures[t_Normal].Sample(s_anisotropic, input.tex1);

    float3 N = getBlueFromOrangeNormalMap(NormalMapTex);

    float4 DecalDirtmapTex = shaderTextures[t_DecalDirtmap].Sample(s_anisotropic, input.tex1 * float2(4, 4));
    float4 DecalMaskTex = shaderTextures[t_DecalMask].Sample(s_anisotropic, input.tex1);
    float4 DecalNormal = shaderTextures[t_DecalNormal].Sample(s_anisotropic, input.tex1);
    float4 DecalSpecular = shaderTextures[t_DecalSpecular].Sample(s_anisotropic, input.tex1);

    float4 SkinMaskTex = shaderTextures[t_SkinMask].Sample(s_anisotropic, input.tex1);

    //VS_OUTPUT input, bool FrontFace : SV_IsFrontFace) :

	//	Get the inputs...
    basis = float3x3(normalize(input.tangent), normalize(input.binormal), normalize(input.normal));;

    //return float4(float(1 - 1 * base_colour.a).xxx, 1);

    ps30_get_shared_inputs(eye_vector, light_vector, base_colour, normal, roughness, metalness, occlusion, /* basis, */input, faction_colours);

	////moving to allow blending
    //float3 pixel_normal = normalize(mul(normalSwizzle_UPDATED(N), basis));

    //return float4((pixel_normal + float3(1, 1, 1)) / 2, 1);
    //N = normalSwizzle_UPDATED(N);
    //oat3 pixel_normal = normalize(mul(normalize(normal), basis));
    float3 pixel_normal = getPixelNormal(input);

    //return float4(get_environment_colour(pixel_normal, 0.7), 1);

    // Attilla system
    //float3x3 basis2 = float3x3(normalize(input.tangent.xyz), normalize(input.binormal.xyz), normalize(input.normal.xyz));
    //float3 pixel_normal = normalize(mul(normalize(N), basis2));

    //  Create the standard material.  This is what gets written to the gbuffer...
    R2_5_StandardLightingModelMaterial_For_GBuffer standard_mat_compressed = R2_5_create_standard_lighting_material_for_gbuffer(base_colour.rgb, pixel_normal, roughness, metalness, occlusion);

	//	Create the uncompressed material.  This is what is read from the gbuffer...
    R2_5_StandardLightingModelMaterial_For_Lighting slm_uncompressed = R2_5_get_slm_for_lighting(standard_mat_compressed);

	//	Apply faction colours...
    //apply_faction_colours(slm_uncompressed.Diffuse_Colour.rgb, input.tex1.xy, faction_colours);

    //  Light the pixel...
    float3 sun = get_sun_colour();

    slm_uncompressed.SSAO = 1.0;
    if (use_AO)
        slm_uncompressed.SSAO = getSSAO(input);

    slm_uncompressed.Shadow = getShadow(input);
    //slm_uncompressed.Shadow = 1.0

    float3 hdr_linear_col = standard_lighting_model_directional_light(
    //2.f.xxx * LightData[0].lightColor.rgb * LightData[0].radiannce,
                 get_sun_colour() * LightData[0].radiannce.xxx,
    light_vector, eye_vector, slm_uncompressed);

    //  Tone-map the pixel...
    float3 ldr_linear_col = saturate(tone_map_linear_hdr_to_linear_ldr(hdr_linear_col, Tone_Map_Black, Tone_Map_White));

    //  Return gamma corrected value...

    //return float4(hdr_linear_col, 1.0f);
    return float4(hdr_linear_col, 1.0f);
};
#endif