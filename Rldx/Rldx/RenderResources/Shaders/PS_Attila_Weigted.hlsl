#include "include\Attila_Lib.hlsli"

//struct PS_OUTPUT
//{
//    float4 Ldr_Color : SV_Target0;
//    float4 Bloom_Cxtract : SV_Target1;
//};

float2 ScreenToUV(float4 screenPos /*, float2 texSize*/)
{
    float4 startFrag = screenPos;
    // Perform the perspective divide.
    startFrag.xyz /= startFrag.w;
       // Convert the screen-space XY coordinates to UV coordinates.
    startFrag.xy = startFrag.xy * 0.5 + 0.5;
       // Convert the UV coordinates to fragment/pixel coordnates.
    //startFrag.xy *= texSize;

    return startFrag.xy;

}

float4 main(in PixelInputType input) : SV_Target0{   
    
	// sample textures
    float4 specular_colour = shaderTextures[t_Specular].Sample(s_anisotropic, input.tex1);
    float4 diffuse_colour = shaderTextures[t_Diffuse].Sample(s_anisotropic, input.tex1);
    float4 GlossTex = shaderTextures[t_GlossMap].Sample(s_anisotropic, input.tex1);
    float4 MaskTex = float4(0, 0, 0, 0); // TODO: fix masking thing in VMDs
    
    shaderTextures[t_Mask].Sample(s_anisotropic, input.tex1);
    float4 NormalMapTex = shaderTextures[t_Normal].Sample(s_anisotropic, input.tex1);

    float4 DecalDirtmapTex = shaderTextures[t_DecalDirtmap].Sample(s_anisotropic, input.tex1 * float2(4, 4));
    float4 DecalMaskTex = shaderTextures[t_DecalMask].Sample(s_anisotropic, input.tex1);
    float4 DecalNormal = shaderTextures[t_DecalNormal].Sample(s_anisotropic, input.tex1);
    float4 DecalSpecular = shaderTextures[t_DecalSpecular].Sample(s_anisotropic, input.tex1);
   
    
#if 0
    // BEGIN: TODO: test code

    float4 uv_rect_coords = decal_uv_rect;
    float2 decal_top_left = uv_rect_coords.xy;
    float2 decal_dimensions = uv_rect_coords.zw - uv_rect_coords.xy;

    float2 decal_uv = (input.tex1 - decal_top_left) / decal_dimensions;

    float4 DecalDiffuse = shaderTextures[t_DecalDiffuse].Sample(s_AnisoClamp, decal_uv);

    //float4 DecalMaskTex = shaderTextures[t_DecalMask].Sample(s_anisotropic, input.tex2);

    float4 decal_color;
    float decal_reflectivity;
    float decal_index;

    float4 coords = float4(0, 0, 8.0, 8.0);

#endif

    if (alpha_used)
    {
        alpha_test(diffuse_colour.a);
    }

    specular_colour.rgb = pow(specular_colour.rgb, 2.2);
    diffuse_colour.rgb = pow(diffuse_colour.rgb, 2.2);

    float3 eye_vector = normalize(-input.viewDirection);
    float3 light_vector = normalize(-lightData[0].lightDirection);

    
    
   //specular_colour.rgb = pow(specular_colour.rgb, 2.2);
   //diffuse_colour.rgb = pow(diffuse_colour.rgb, 2.2);

   //float s_smoothness = GlossTex.x;

    //float3 eye_vector = normalize(input.mViewI[3].xyz - input.Wpos.xyz);

    //float smoothness = pow(GlossTex.x, (1.7 * smoothness_curve));
    //float reflectivity = pow(GlossTex.y, (1 * reflect_curve));

    //float smoothness = _gamma(smoothness_curve * reflect_curve);

    //float smoothness = pow(GlossTex.x, 1/2 * smoothness_curve);
    float smoothness = pow(GlossTex.x, 2.2);
    //float smoothness = GlossTex.x;
    
    
    
    //float reflectivity = pow(GlossTex.y, 2 * reflect_curve);
    float reflectivity = pow(GlossTex.y, 2.2);
    //float reflectivity = GlossTex.y;

    
    
    float mask_p1 = MaskTex[0]; //   puiMaskIndices[0]
    float mask_p2 = MaskTex[1];
    float mask_p3 = MaskTex[2];

    
    if (bDoFactionColoring)
    {
		diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * get_adjusted_faction_colour(_linear(colorTable[0].rgb)), mask_p1);
		diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * get_adjusted_faction_colour(_linear(colorTable[1].rgb)), mask_p2);
		diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * get_adjusted_faction_colour(_linear(colorTable[2].rgb)), mask_p3);
	}
    else
    {
        diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * _linear(tint_color1.rgb), mask_p1);
        diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * _linear(tint_color1.rgb), mask_p2);
        diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * _linear(tint_color1.rgb), mask_p3);
    }
    
    //if (bDoFactionColoring)
    //{
    //    diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * get_adjusted_faction_colour(_linear(color1.rgb)), mask_p1);
    //    diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * get_adjusted_faction_colour(_linear(color2.rgb)), mask_p2);
    //    diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * get_adjusted_faction_colour(_linear(color3.rgb)), mask_p3);
    //}
    
    float3 N = getBlueFromOrangeNormalMap(NormalMapTex);    

    
    
#if 1
    bool b_do_decal = true;
    if (b_do_decal)
    {
        ps_common_blend_decal(
            diffuse_colour, N, specular_colour.rgb, reflectivity,
            diffuse_colour, N, specular_colour.rgb, reflectivity, input.tex1.xy, 0, decal_uv_rect, 1.0
        );

    }
#endif
#if 1
    //float2 tex = input.tex1;
    //float reflect = reflectivity;

    float2 decal_uv = input.tex1; // * float2(u_div, v_div);

    ps_common_blend_dirtmap(
        diffuse_colour,
        N,
        specular_colour.rgb,
        reflectivity,
        diffuse_colour,
        N,
        specular_colour.rgb,
        reflectivity,
        decal_uv,
        float2(u_div, v_div) //        float2(0, 0)
    );
#endif
    float3x3 basis = float3x3(normalize(input.tangent.xyz), normalize(input.binormal.xyz), normalize(input.normal.xyz));
    float3 pixel_normal = normalize(mul(normalize(N), basis));

    StandardLightingModelMaterial_UPDATED standard_mat =
        create_standard_lighting_material_UPDATED(
            diffuse_colour.rgb,
            specular_colour.rgb,
            pixel_normal,
            smoothness,
            reflectivity,
            float4(normalize(input.Wpos.xyz), 1)
        );

    if (use_AO)
        standard_mat.SSAO = getSSAO(input);
    else
        standard_mat.SSAO = 1.0;

    const float shadow = getShadow(input);

    // no shadowing:
    standard_mat.Shadow = 1.0f; //shadow; //    1.0f; //lerp(1.0f, shadow, shadow_factor);

    float3 hdr_linear_col = standard_lighting_model_directional_light_UPDATED(      
        lightData[0].radiance * lightData[0].color.rgb,
	    normalize(light_vector),
	    normalize(eye_vector),
	    standard_mat

    );

    float alpha = 1.0f;
    if (all_transparent)
    {
        alpha = 0.4f;
    }    


    float3 ldr_linear_color = Uncharted2ToneMapping(hdr_linear_col);
    //return float4(hdr_linear_col, alpha);
    return float4(ldr_linear_color, alpha);
};