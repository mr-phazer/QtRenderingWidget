#include "Attila_Lib.hlsli"
//#include "shadows.hlsli"

//struct PS_OUTPUT
//{
//    float4 Ldr_Color : SV_Target0;
//    float4 Bloom_Cxtract : SV_Target1;
//};

float4 main(in PixelInputType input) : SV_TARGET0
{       
     // sample textures
    float4 specular_colour = shaderTextures[t_Specular].Sample(s_anisotropic, input.tex1);
    float4 diffuse_colour = shaderTextures[t_Diffuse].Sample(s_anisotropic, input.tex1);
    float4 GlossTex = shaderTextures[t_GlossMap].Sample(s_anisotropic, input.tex1);
    float4 MaskTex = shaderTextures[t_Mask].Sample(s_anisotropic, input.tex1);

    float4 SkinMaskTex = shaderTextures[t_SkinMask].Sample(s_anisotropic, input.tex1);

    alpha_test(diffuse_colour.a);

    float3 pixel_normal = getPixelNormal(input);

    float3 eye_vector = -normalize(input.viewDirection);
    float3 light_vector = -normalize(LightData[0].lightDirection);
    float smoothness = pow(GlossTex.x, 2.2 * smoothness_curve);   
    float reflectivity = pow(GlossTex.y, 2.2 * reflect_curve);

    float mask_p1 = MaskTex.r; // TODO: shouold this not R, G, B ??
    float mask_p2 = MaskTex.g;
    float mask_p3 = MaskTex.b;

    //if (b_do_decal)
    //{
    //    ps_common_blend_decal(diffuse_colour, N, specular_colour.rgb, reflectivity, diffuse_colour, N, specular_colour.rgb, reflectivity, input.TexCoord.xy, 0, vec4_uv_rect, 1.0);
    //}

    //float4 Np = (tex2D(s_normal_map, input.TexCoord.xy));
    //float3 N = pixel_normal;

    //if (b_do_decal)
    //{
    //    ps_common_blend_decal(diffuse_colour, N, specular_colour.rgb, reflectivity, diffuse_colour, N, specular_colour.rgb, reflectivity, input.TexCoord.xy, 0, vec4_uv_rect, 1.0);
    //}

    //if (b_do_dirt)
    //{
    //    ps_common_blend_dirtmap(diffuse_colour, N, specular_colour.rgb, reflectivity, diffuse_colour, N, specular_colour.rgb, reflectivity, input.TexCoord.xy, float2(f_uv_offset_u, f_uv_offset_v));
    //}

    //float3 pixel_normal = normalize(mul(normalize(N), basis));

    //specular_colour.rgb = _linear(specular_colour.rgb);
    //diffuse_colour.rgb = _linear(diffuse_colour.rgb);

    specular_colour.rgb = pow(specular_colour.rgb, 2.2);
    diffuse_colour.rgb = pow(diffuse_colour.rgb, 2.2);

    SkinLightingModelMaterial skin_mat = create_skin_lighting_material(
    float2(smoothness, reflectivity),
    SkinMaskTex.xyz,
    diffuse_colour.rgb,
    specular_colour.rgb,
    pixel_normal,
    float4(input.Wpos.xyz, 1)
    );

            // View Space -> UV space, get AO, store AO in material

    float AO = getSSAO(input);

    //return float4(AO.xxx, 1);

    //if (use_AO)
    //    skin_mat.SSAO = AO;
    //else
    skin_mat.SSAO = 1.0;

    float2 shadow_buffer_UV;
    shadow_buffer_UV.xy = input.position.xy * screenRes.zw;
    float shadow = float4(Shadow_mapTexture.Sample(s_point, shadow_buffer_UV.xy).rgb, 1);

    skin_mat.Shadow = lerp(1.0f, shadow, shadow_factor);

    float3 hdr_linear_col =

    skin_lighting_model_directional_light(
        0.5 * LightData[0].lightColor.rgb * LightData[0].radiannce,
        normalize(light_vector),
        normalize(eye_vector),
        skin_mat
    );

    //float3 ldr_linear_col = saturate(tone_map_linear_hdr_pixel_value(hdr_linear_col));
    float3 ldr_linear_col = saturate(exposure * Uncharted2ToneMapping(hdr_linear_col));

    float alpha = 1.0f;
    if (all_transparent)
    {
        alpha = 0.4f;
    }

    //o.Ldr_Color = float4( /*_gamma*/(hdr_linear_col), 1);
    float3 color = float4( /*_gamma*/(hdr_linear_col), 1);

    //float brightness = dot(hdr_linear_col.rgb, float3(0.2126, 0.7152, 0.0722));
    //if (brightness > 1.0f)
    //    o.Bloom_Cxtract = float4(0, 0, 0, 0);
    ////float4(hdr_linear_col, 1);
    //else
    //    o.Bloom_Cxtract = float4(0, 0, 0, 0);

    return float4(color.rgb, 1);

}