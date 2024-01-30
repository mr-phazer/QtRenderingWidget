//#include "layout.hlsli"
//#include "textures.hlsli"
//#include "PS_ConstBuffers.hlsli"
//#include "tone_mapping.hlsli"
#include "pbr_lib.hlsli"
#include "util.hlsli"

//float getShadow(in PixelInputType input)
//{
//    float2 shadowTexCoords;
//    shadowTexCoords.x = 0.5f + (input.lightSpaceposition.x / input.lightSpaceposition.w * 0.5f);
//    shadowTexCoords.y = 0.5f - (input.lightSpaceposition.y / input.lightSpaceposition.w * 0.5f);
//    float pixelDepth = input.lightSpaceposition.z / input.lightSpaceposition.w;

//    float shadowFactor = 1;

//    float3 NdotL = dot(input.normal.xyz, LightData[0].lightDirection);

//// Check if the pixel texture coordinate is in the view frustum of the
//// light before doing any shadow work.
//    if ((saturate(shadowTexCoords.x) == shadowTexCoords.x) &&
//    (saturate(shadowTexCoords.y) == shadowTexCoords.y) &&
//    (pixelDepth > 0))
//    {
//// Use an offset value to mitigate shadow artifacts due to imprecise
//// floating-point values (shadow acne).
////
//// This is an approximation of epsilon * tan(acos(saturate(NdotL))):
//        float margin = acos(saturate(NdotL));
//        float margin_f = tan(acos(saturate(NdotL)));
//#ifdef LINEAR
//// The offset can be slightly smaller with smoother shadow edges.
//float epsilon = 0.0005 / margin;
//#else
//        float epsilon = 0.0005 / margin_f;
//#endif
//// Clamp epsilon to a fixed range so it doesn't go overboard.
//        epsilon = clamp(epsilon, 0, 0.1);

//// Use the SampleCmpLevelZero Texture2D method (or SampleCmp) to sample from
//// the shadow map, just as you would with Direct3D feature level 10_0 and
//// higher.  Feature level 9_1 only supports LessOrEqual, which returns 0 if
//// the pixel is in the shadow.
//        float bias = 0.005 * tan(acos(NdotL));
//        shadowFactor = float(Shadow_mapTexture.SampleCmpLevelZero(
//        ShadowSampler,
//    shadowTexCoords,
//    pixelDepth + epsilon
//    //pixelDepth - bias
//    ).r
//    );
//    };

//    return shadowFactor;

//}
/////////////////////////
// parameters
/////////////////////////
//float pi = 3.14159265;
//float one_over_pi = 1 / 3.14159265;
float real_approx_zero = 0.001f;

////////////////////////////////////////////////////////////////////////////////////////////////////
//	Colorimetry Functions
////////////////////////////////////////////////////////////////////////////////////////////////////
float _toLinear2(in float fGamma)
{
    return pow(max(fGamma, 0.0001f), 2.2f);
}

float3 _toLinear2(in float3 vGamma)
{
    return pow(max(vGamma, 0.0001f), 2.2f);
}

float3 _gamma(in float3 vLinear)
{
    return pow(max(vLinear, 0.0001f), 1.0f / 2.2f);
}

float _gamma(in float fLinear)
{
    return pow(max(fLinear, 0.0001f), 1.0f / 2.2f);
}

float get_diffuse_scale_factor()
{
    return 0.004f;
}

float get_game_hdr_lighting_multiplier()
{
    return 5000.0f;
}

float get_luminance(in float3 colour)
{
    float3 lumCoeff = float3(0.299, 0.587, 0.114);
    float luminance = dot(colour, lumCoeff);
    return saturate(luminance);
}

float3 get_adjusted_faction_colour(in float3 colour)
{
    float3 fc = colour;
    float lum = get_luminance(fc);
    float dark_scale = 1.5;
    float light_scale = 0.5;

    fc = fc * (lerp(dark_scale, light_scale, lum));

    return fc;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	Conversion Functions
////////////////////////////////////////////////////////////////////////////////////////////////////

float3 texcoordEnvSwizzle(in float3 ref)
{
    //return float3(ref.x, ref.y, -ref.z);
    return float3(ref.x, ref.y, ref.z);
#ifdef FXCOMPOSER
    return float3( ref.x, ref.y, -ref.z );
#else
    return -float3(ref.x, -ref.z, ref.y);
#endif
}

float3 normalSwizzle(in float3 ref)
{
    return ref.xyz;
#ifdef FXCOMPOSER
	return ref.xyz;
#else
    return float3(ref.y, ref.x, ref.z);
#endif
}

//float3 normalSwizzle_UPDATED(in float3 ref)
//{
//#ifdef FXCOMPOSER
//	return ref.xyz;
//#else
//    return float3(ref.x, ref.z, ref.y);
//#endif
//}

float cos2sin(float x)
{
    return sqrt(1 - x * x);
}

float cos2tan2(float x)
{
    return (1 - x * x) / (x * x);
}

float contrast(float _val, float _contrast)
{
    _val = ((_val - 0.5f) * max(_contrast, 0)) + 0.5f;
    return _val;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	Forward Declarations
////////////////////////////////////////////////////////////////////////////////////////////////////

float3 tone_map_linear_hdr_pixel_value(in float3 linear_hdr_pixel_val);
float4 HDR_RGB_To_HDR_CIE_Log_Y_xy(in float3 linear_colour_val);
float4 tone_map_HDR_CIE_Log_Y_xy_To_LDR_CIE_Yxy(in float4 hdr_LogYxy);
float4 LDR_CIE_Yxy_To_Linear_LDR_RGB(in float4 ldr_cie_Yxy);
float get_scurve_y_pos(const float x_coord);

////////////////////////////////////////////////////////////////////////////////////////////////////
//	Lighting Functions
////////////////////////////////////////////////////////////////////////////////////////////////////

float get_env_scaler()
{
    return 7.f * env_radiance * 0.2;
}

float3 get_environment_colour_UPDATED(in float3 direction, in float lod)
{
    return (tex_cube_specular.SampleLevel(s_anisotropic, texcoordEnvSwizzle(direction), lod) * get_env_scaler()).rgb;
    //return texCUBElod(s_environment_map, float4(texcoordEnvSwizzle(direction), lod)).rgb;

}

float3 cube_ambient(in float3 N)
{
    //return tex_cube_diffuse.SampleLevel(SamplerLinear, N, 0);
    return (tex_cube_diffuse.SampleLevel(SamplerLinear, texcoordEnvSwizzle(N), 0) * get_env_scaler()).rgb;
    //return tex_cube_specular.Sample(SamplerLinear, N);
    //return texCUBE(s_ambient, texcoordEnvSwizzle(N)).rgb;
}

float2 phong_diffuse(in float3 N,
						in float3 L)
{
    const float factor = max(0.0f, dot(N, -L));
    return float2(factor, (factor > 0.0f));
}

float phong_specular(in float3 I, in float3 N, in float shininess,
						in float3 L)
{
    const float3 R = reflect(L, N);
    return saturate(pow(max(0.0f, dot(R, -I)), shininess)); // TODO "2*" is test code
}

float aniso_specular(in float3 I, float3 N, in float3 T, in float shininess,
						in float3 L)
{
    float3 nH = normalize(I + L);
    float3 nT = normalize(T);
    nT = normalize(nT - N * dot(N, nT));
    float spec = pow(sqrt(1 - (pow(dot(nT, nH), 2))), shininess);

    return spec;
}

float blinn_specular(in float3 I, in float3 N, in float shininess,
						in float3 L)
{
    shininess = shininess * 4.0;
    float3 H = normalize(I + L);
    const float3 R = reflect(L, N);
    return saturate(pow(max(0.0f, dot(N, -H)), shininess));
}

float blinn_phong_specular(in float dotNH, in float SpecularExponent)
{

    float D = pow(dotNH, SpecularExponent) * (SpecularExponent + 1.0f) / 2.0f;
    return D;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//	Cook Torrance Model
////////////////////////////////////////////////////////////////////////////////////////////////////

float beckmann_distribution(in float dotNH, in float SpecularExponent)
{
    float invm2 = SpecularExponent / 2.0f;
    float D = exp(-cos2tan2(dotNH) * invm2) / pow(dotNH, 4.0f) * invm2;
    return D;
}

float3 fresnel_optimized(in float3 R, in float c)
{
    float3 F = lerp(R, saturate(60.0f * R), pow(1.0f - c, 4.0f));
    return F;
}

float3 fresnel_full(in float3 R, in float c)
{
    float3 n = (1 + sqrt(R)) / (1 - sqrt(R));
    float3 FS = (c - n * sqrt(1 - pow(cos2sin(c) / n, 2))) / (c + n * sqrt(1 - pow(cos2sin(c) / n, 2)));
    float3 FP = (sqrt(1 - pow(cos2sin(c) / n, 2)) - n * c) / (sqrt(1 - pow(cos2sin(c) / n, 2)) + n * c);
    return (FS * FS + FP * FP) / 2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	decal / dirt
////////////////////////////////////////////////////////////////////////////////////////////////////

//void ORG__ps_common_blend_decal(in float4 colour, in float3 normal, in float3 specular, in float reflectivity, out float4 ocolour, out float3 onormal, out float3 ospecular, out float oreflectivity, in float2 uv, in float decal_index, in float4 uv_rect_coords, in float valpha)
//{
//    float2 decal_top_left = uv_rect_coords.xy;
//    float2 decal_dimensions = uv_rect_coords.zw - uv_rect_coords.xy;

//    float2 decal_uv = (uv - decal_top_left) / decal_dimensions;

//    float4 decal_diffuse;
//    float3 decal_normal;

//    decal_diffuse = tex2D(s_decal_diffuse, decal_uv).rgba;
//    decal_normal = normalSwizzle_UPDATED((tex2D(s_decal_normal, decal_uv).xyz * 2.0f) - 1.0f);
//    float decal_mask = tex2D(s_decal_mask, uv).a;

//    float decalblend = decal_mask * decal_diffuse.a * valpha;
//    oreflectivity = lerp(reflectivity, reflectivity * 0.5, (decalblend));
//    ocolour = float4(1, 1, 1, 1);
//    onormal = float4(0, 0, 1, 1);
//    ospecular = lerp(specular, decal_diffuse.rgb, decalblend);

//    ocolour.rgb = lerp(colour.rgb, decal_diffuse.rgb, decalblend);

//    onormal.xyz = lerp(onormal.xyz, decal_normal.rgb, decalblend);
//    onormal.xyz = float3(normal.xy + onormal.xy, normal.z);
//}

void ps_common_blend_decal(
    in float4 colour, in float3 normal, in float3 specular, in float reflectivity, 
    out float4 ocolour, out float3 onormal, out float3 ospecular, out float oreflectivity, 

    in float2 uv, in float decal_index, in float4 uv_rect_coords, in float valpha)
{
    float2 decal_top_left = uv_rect_coords.xy;
    float2 decal_dimensions = uv_rect_coords.zw - uv_rect_coords.xy;

    float2 decal_uv = (uv - decal_top_left) / decal_dimensions;    

    float4 decal_diffuse;
    float3 decal_normal;

    //decal_diffuse = shaderTextures[t_DecalDiffuse].Sample(SamplerLinear, decal_uv).rgba;
    decal_diffuse = shaderTextures[t_DecalDiffuse].Sample(s_AnisoClamp, decal_uv);
    decal_diffuse.rgb = _linear(decal_diffuse.rgb);
    //decal_normal = normalSwizzle_UPDATED(
    //    (shaderTextures[t_DecalNormal].Sample(SamplerLinear, decal_uv).xyz * 2.0f) - 1.0f
    //);
    

    decal_normal = getBlueFromOrangeNormalMap(shaderTextures[t_DecalNormal].Sample(SamplerLinear, decal_uv)).xyz;

    float decal_mask = 1.0f;
    //shaderTextures[t_DecalMask].Sample(SamplerLinear, uv).a;

    float decalblend = decal_mask * decal_diffuse.a * valpha;
    oreflectivity = lerp(reflectivity, reflectivity * 0.5, (decalblend));

    ocolour = float4(1, 1, 1,1);
    onormal = float3(0, 0, 1);

    ospecular = lerp(specular, decal_diffuse.rgb, decalblend);

    ocolour.rgb = lerp(colour.rgb, decal_diffuse.rgb, decalblend);

    onormal.xyz = lerp(onormal.xyz, decal_normal.rgb, decalblend);
    onormal.xyz = float3(normal.xy + onormal.xy, normal.z);
}

void ps_common_blend_dirtmap(
    in out float4 colour,
    in out float3 normal, in float3 specular,
    in out float reflectivity,
    out float4 ocolour,
    out float3 onormal,
    out float3 ospecular,
    out float oreflectivity,
    in float2 uv,
    in float2 uv_offset2
)
{
    // USER CODE
    int i_random_tile_u = 1, i_random_tile_v = 1;

    float2 uv_offset = float2(u_div, v_div) * float2(i_random_tile_u, i_random_tile_v);

    float mask_alpha = shaderTextures[t_DecalDirtmask].Sample(s_anisotropic, uv).a;
    float4 dirtmap = shaderTextures[t_DecalDirtmap].Sample(s_anisotropic,
    ((uv) + uv_offset) *
    float2(decal_u, decal_v)
    //float2(4, 4)
    ).xyzw;

    float d_strength = 0.5;

    //// USER CODE: Original:   //float2 dirt_normal = (float2(dirtmap.x, dirtmap.y) * 2) - 1;
    float3 dirt_normal = (dirtmap.xyz * 2) - 1;

    float dirt_alpha = dirtmap.w;
    float dirt_alpha_blend = mask_alpha * dirt_alpha * d_strength;

    float3 dirt_colour = float3(0.03, 0.03, 0.02);
    ocolour = colour;
    onormal = normal;

    ocolour.rgb = lerp(colour.rgb, dirt_colour, dirt_alpha_blend);

    ospecular = lerp(specular, dirt_colour, dirt_alpha_blend);

    //onormal.xz += (dirt_normal.xy * mask_alpha * d_strength);
    //onormal.xz += (dirt_normal.xy * mask_alpha * 1.0);

    // USER CODE
    onormal.xy += (dirt_normal.xy * mask_alpha * 1.0);

    onormal = normalize(onormal);

    oreflectivity = reflectivity;

}

//void ps_common_blend_dirtmap(in out float4 colour, in out float3 normal, in float3 specular, in out float reflectivity, out float4 ocolour, out float3 onormal, out float3 ospecular, out float oreflectivity, in float2 uv, in float2 uv_offset)
//{
//    uv_offset = uv_offset * float2(i_random_tile_u, i_random_tile_v);

//    float mask_alpha = tex2D(s_decal_dirtmask, uv).a;
//    float4 dirtmap = tex2D(s_decal_dirtmap, ((uv) + uv_offset) * float2(f_uv2_tile_interval_u, f_uv2_tile_interval_v)).xyzw;

//    float d_strength = 1.0;

//    float2 dirt_normal = (float2(dirtmap.x, dirtmap.y) * 2) - 1;

//    float dirt_alpha = dirtmap.w;
//    float dirt_alpha_blend = mask_alpha * dirt_alpha * d_strength;

//    float3 dirt_colour = float3(0.03, 0.03, 0.02);
//    ocolour = colour;
//    onormal = normal;

//    ocolour.rgb = lerp(colour.rgb, dirt_colour, dirt_alpha_blend);

//    ospecular = lerp(specular, dirt_colour, dirt_alpha_blend);

//    onormal.xz += (dirt_normal.xy * mask_alpha * d_strength);
//    onormal = normalize(onormal);

//    oreflectivity = reflectivity;
//}

//void ps_common_blend_vfx(in out float4 colour, in out float3 normal, in float3 specular, in out float reflectivity, out float4 ocolour, out float3 onormal, out float3 ospecular, out float oreflectivity, in float2 uv, in float2 uv_offset)
//{
//    uv_offset = uv_offset * float2(i_random_tile_u, i_random_tile_v);

//    float4 dirtmap = tex2D(s_decal_dirtmap, ((uv) + uv_offset) * float2(f_uv2_tile_interval_u, f_uv2_tile_interval_v)).xyzw;

//    ocolour = float4(lerp(colour.rgb, dirtmap.rgb, dirtmap.a), 1);

//    onormal = normal;
//    ospecular = specular;
//    oreflectivity = reflectivity;
//}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	SKIN_LIGHTING_MODEL	-
////////////////////////////////////////////////////////////////////////////////////////////////////

struct SkinLightingModelMaterial
{
    float RimMask;
    float SubSurfaceStrength;
    float BackScatterStrength;
    float Gloss;
    float SpecularLevel;
    float3 specular_colour;
    float3 Color;
    float3 Normal;
    float Depth;
    float Shadow;
    float SSAO;
};

SkinLightingModelMaterial create_skin_lighting_material(in float2 _MaterialMap, in float3 _SkinMap, in float3 _Color, in float3 _specular_colour, in float3 _Normal, in float4 _worldposition)
{

    SkinLightingModelMaterial material;
    material.Gloss = _MaterialMap.x;
    material.SpecularLevel = _MaterialMap.y;
    material.RimMask = _SkinMap.x;
    material.SubSurfaceStrength = _SkinMap.y;
    material.BackScatterStrength = _SkinMap.z;
    material.Color = _Color;
    material.specular_colour = _specular_colour;
    material.Normal = normalize(_Normal);
    material.Depth = 1.0;
    material.Shadow = 1.0f;
    material.SSAO = 1.0;

    return material;
}

float3 skin_shading(in float3 L, in float3 N, in float3 V, in float sss_strength, in float3 colour1, in float3 colour2)
{
    float ndotl = dot(N, -L);

    float3 diff1 = ndotl * saturate(((ndotl * 0.8) + 0.3) / 1.44);
    float3 diff2 = colour1 * (saturate(((ndotl * 0.9) + 0.5) / 1.44)) * saturate(1 - ((diff1 + 0.3)));
    float3 diff3 = colour2 * (saturate(((ndotl * 0.3) + 0.3) / 2.25)) * (1 - diff1) * (1 - diff2);

    float3 mix = (diff1 + diff2 + diff3);

    float3 blendedDiff = lerp(float3(ndotl, ndotl, ndotl), (mix), sss_strength);
    return saturate(float3(blendedDiff));
}

float get_skin_dlight_diffuse_scaler()
{
    return 0.9f;
}

float get_skin_dlight_specular_scaler()
{
    return 2.0f;
}

float get_skin_dlight_rim_scaler()
{
    return 1.0f;
}

float3 skin_lighting_model_directional_light(in float3 LightColor, in float3 normalised_light_dir, in float3 normalised_view_dir, in SkinLightingModelMaterial skinlm_material)
{
    LightColor *= get_game_hdr_lighting_multiplier();

    normalised_light_dir = -normalised_light_dir;

    float3 diffuse_scale_factor = get_diffuse_scale_factor().xxx;

    float normal_dot_light_dir = max(dot(skinlm_material.Normal, -normalised_light_dir), 0);

    float3 dlight_diffuse = skinlm_material.Color.rgb * skin_shading(normalised_light_dir, skinlm_material.Normal, normalised_view_dir, skinlm_material.SubSurfaceStrength, _linear(float3(0.612066, 0.456263, 0.05)), _linear(float3(0.32, 0.05, 0.006))) * LightColor * diffuse_scale_factor;
    dlight_diffuse *= get_skin_dlight_diffuse_scaler();

    float backscatter = pow(saturate(dot(skinlm_material.Normal, normalised_light_dir)), 2.0) * pow(saturate(dot(normalised_view_dir, -normalised_light_dir)), 4.0f) * skinlm_material.BackScatterStrength;

    float3 backscatter_colour = lerp((LightColor), (LightColor * _linear(float3(0.7, 0, 0))),
    skinlm_material.SubSurfaceStrength) * diffuse_scale_factor * backscatter * skinlm_material.Shadow;

    dlight_diffuse += (skinlm_material.Color.rgb * backscatter_colour * get_skin_dlight_diffuse_scaler());

    float3 env_light_diffuse = skinlm_material.Color.rgb * cube_ambient(skinlm_material.Normal).rgb * 0.1;

    //return float4(env_light_diffuse, 1); // TODO: remove debugging code

    float kspec = phong_specular(normalised_view_dir, skinlm_material.Normal, lerp(1, 128, (skinlm_material.Gloss * skinlm_material.Gloss)), normalised_light_dir);

    float3 dlight_specular = skinlm_material.SpecularLevel * kspec * LightColor * diffuse_scale_factor * get_skin_dlight_specular_scaler();

    float3 reflected_view_vec = reflect(-normalised_view_dir, skinlm_material.Normal);

    float3 rim_env_colour = cube_ambient(reflected_view_vec).rgb * 0.001;

    float rimfresnel = saturate(1 - (dot(-normalised_view_dir, skinlm_material.Normal)));

    float riml = saturate(pow((rimfresnel), 2)) * skinlm_material.RimMask * 1.5 * get_skin_dlight_rim_scaler(); //1.5 is a scalar in order that we can get a brighter rim by setting the texture to be white.  Previously the maximum brightness was quite dull.

    float upness = max(dot(normalize(skinlm_material.Normal + _linear(float3(0, 0.75, 0))), _linear(float3(0, 1, 0))), 0);

    float3 env_light_specular = (riml * upness * rim_env_colour);

    float shadow_attenuation = skinlm_material.Shadow;

    return (skinlm_material.SSAO * (env_light_diffuse + env_light_specular)) + (shadow_attenuation * (dlight_specular + dlight_diffuse));
}

//float3 skin_lighting_model_directional_light(in float3 LightColor, in float3 normalised_light_dir, in float3 normalised_view_dir, in SkinLightingModelMaterial skinlm_material)
//{
//    LightColor *= get_game_hdr_lighting_multiplier();

//    normalised_light_dir = -normalised_light_dir;

//    float3 diffuse_scale_factor = get_diffuse_scale_factor().xxx;

//    float normal_dot_light_dir = max(dot(skinlm_material.Normal, -normalised_light_dir), 0);

//    float3 dlight_diffuse = skinlm_material.Color.rgb * skin_shading(normalised_light_dir, skinlm_material.Normal, normalised_view_dir, skinlm_material.SubSurfaceStrength, float3(0.612066, 0.456263, 0.05), float3(0.32, 0.05, 0.006)) * LightColor * diffuse_scale_factor;
//    dlight_diffuse *= get_skin_dlight_diffuse_scaler();

//    float backscatter = pow(saturate(dot(skinlm_material.Normal, normalised_light_dir)), 2.0) * pow(saturate(dot(normalised_view_dir, -normalised_light_dir)), 4.0f) * skinlm_material.BackScatterStrength;

//    float3 backscatter_colour = lerp((LightColor), (LightColor * float3(0.7, 0, 0)), skinlm_material.SubSurfaceStrength) * diffuse_scale_factor * backscatter * skinlm_material.Shadow;

//    dlight_diffuse += (skinlm_material.Color.rgb * backscatter_colour * get_skin_dlight_diffuse_scaler());

//    float3 env_light_diffuse = skinlm_material.Color.rgb * cube_ambient(skinlm_material.Normal).rgb;

//    float kspec = phong_specular(normalised_view_dir, skinlm_material.Normal, lerp(1, 128, (skinlm_material.Gloss * skinlm_material.Gloss)), normalised_light_dir);

//    float3 dlight_specular = skinlm_material.SpecularLevel * kspec * LightColor * diffuse_scale_factor * get_skin_dlight_specular_scaler();

//    float3 reflected_view_vec = reflect(-normalised_view_dir, skinlm_material.Normal);

//    float3 rim_env_colour = cube_ambient(reflected_view_vec).rgb;

//    float rimfresnel = saturate(1 - (dot(-normalised_view_dir, skinlm_material.Normal)));

//    float riml = saturate(pow((rimfresnel), 2)) * skinlm_material.RimMask * 1.5 * get_skin_dlight_rim_scaler(); //1.5 is a scalar in order that we can get a brighter rim by setting the texture to be white.  Previously the maximum brightness was quite dull.

//    float upness = max(dot(normalize(skinlm_material.Normal + float3(0, 0.75, 0)), float3(0, 1, 0)), 0);

//    float3 env_light_specular = (riml * upness * rim_env_colour);

//    float shadow_attenuation = skinlm_material.Shadow;

//    return (skinlm_material.SSAO * (env_light_diffuse + env_light_specular)) * env_light_radiance +
//    (shadow_attenuation *

//    (dlight_specular + dlight_diffuse) * (env_light_radiance / 4)

//    );

//}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	STANDARD_LIGHTING_MODEL
////////////////////////////////////////////////////////////////////////////////////////////////////

struct StandardLightingModelMaterial_UPDATED
{
    float3 Diffuse_Colour;
    float3 Specular_Colour;
    float3 Normal;
    float Smoothness;
    float Reflectivity;

    float Depth;
    float Shadow;
    float SSAO;
};

StandardLightingModelMaterial_UPDATED create_standard_lighting_material_UPDATED(in float3 _DiffuseColor, in float3 _Specular_Colour, in float3 _Normal, in float _Smoothness, in float _Reflectivity, in float4 _worldposition)
{
    StandardLightingModelMaterial_UPDATED material;

    material.Diffuse_Colour = _DiffuseColor;
    material.Specular_Colour = _Specular_Colour;
    material.Normal = _Normal;
    material.Smoothness = _Smoothness;
    material.Reflectivity = _Reflectivity;
    material.Depth = 1.0f;
    material.Shadow = 1.0f;
    material.SSAO = 1.0f;

    return material;
}

float get_sun_angular_diameter()
{
    const float suns_angular_diameter = 1.0f;

    return radians(suns_angular_diameter);
}

float get_sun_angular_radius()
{
    return 0.5f * get_sun_angular_diameter();
}

float get_error_func_a_value()
{
    return (8.0f * (pi - 3.0f)) / (3 * pi * (4.0f - pi));
}

float erf(float x)
{
    float x_squared = x * x;

    float a = get_error_func_a_value();

    float a_times_x_squared = a * x_squared;

    float numerator = (4.0f * one_over_pi) + (a_times_x_squared);

    float denominator = 1.0f + a_times_x_squared;

    float main_term = -1.0f * x_squared * (numerator / denominator);

    return sign(x) * sqrt(1 - exp(main_term));
}

float erfinv(float x)
{
    float one_over_a = 1.0f / get_error_func_a_value();

    float log_1_minus_x_squared = log(1.0f - (x * x));

    float root_of_first_term = (2.0f * one_over_pi * one_over_a) + (log_1_minus_x_squared * 0.5f);

    float first_term = root_of_first_term * root_of_first_term;

    float second_term = log_1_minus_x_squared * one_over_a;

    float third_term = (2.0f * one_over_pi * one_over_a) + (log_1_minus_x_squared * 0.5f);

    float all_terms = first_term - second_term - third_term;

    return sign(x) * sqrt(sqrt(first_term - second_term) - third_term);
}

float norm_cdf(float x, float sigma)
{
    float one_over_root_two = 0.70710678118654752440084436210485f;

    return 0.5f * (1.0f + erf(x * (1.0f / sigma) * one_over_root_two));
}

float norm_cdf(float x_min, float x_max, float sigma)
{
    float min_summed_area = norm_cdf(x_min, sigma);
    float max_summed_area = norm_cdf(x_max, sigma);

    return max_summed_area - min_summed_area;
}

float norminv_sigma(float x, float area_under_the_graph)
{
    float error_func_x = erfinv((2.0f * area_under_the_graph) - 1);

    float sigma = x / (error_func_x * 1.4142135623730950488016887242097f);

    return sigma;
}

float get_normal_distribution_sigma_about_origin(float area_under_graph_centred_around_origin, float x_half_distance_from_origin)
{
    float area_from_x_neg_infinity = 0.5f + (0.5f * area_under_graph_centred_around_origin);

    return norminv_sigma(x_half_distance_from_origin, area_from_x_neg_infinity);
}

float determine_fraction_of_facets_at_reflection_angle(in float smoothness, in float light_vec_reflected_view_vec_angle)
{
    float sun_angular_radius = get_sun_angular_radius();

    float max_fraction_of_facets = 0.9999f;
    float min_fraction_of_facets = get_diffuse_scale_factor();

    float fraction_of_facets = lerp(min_fraction_of_facets, max_fraction_of_facets, smoothness * smoothness);

    float fraction_of_facets_to_look_for = 0.5f + (fraction_of_facets * 0.5f);

    float sigma = max(norminv_sigma(sun_angular_radius, fraction_of_facets_to_look_for), 0.0001f);

    float proportion_of_facets = norm_cdf(light_vec_reflected_view_vec_angle - sun_angular_radius, light_vec_reflected_view_vec_angle + sun_angular_radius, sigma);

    return proportion_of_facets;
}

float determine_facet_visibility(in float roughness, in float3 normal_vec, in float3 light_vec)
{
    const float n_dot_l = saturate(dot(normal_vec, light_vec));
    const float towards_diffuse_surface = sin(roughness * pi * 0.5f); //	( 0 - 1 ) output...
    const float facet_visibility = lerp(1.0f, n_dot_l, towards_diffuse_surface);

    return facet_visibility;
}

float3 determine_surface_reflectivity(in float3 material_reflectivity, in float roughness, in float3 light_vec, in float3 view_vec)
{
    float fresnel_curve = 10;

    float val1 = max(0, dot(light_vec, -view_vec));

    float val2 = pow(val1, fresnel_curve);

    float fresnel_bias = 0.5f;

    float roughness_bias = 0.98f;

    float smoothness_val = pow(cos(roughness * roughness_bias * pi * 0.5f), fresnel_bias);

    return lerp(material_reflectivity, saturate(60.0f * material_reflectivity), val2 * smoothness_val);
}

float4 plot_standard_lighting_model_test_func(in float4 vpos)
{
    float4 g_vpos_texel_offset = float4(0, 0, 0, 0);
    float4 g_screen_size_minus_one = float4(0, 0, 0, 0);

    vpos -= g_vpos_texel_offset.xxxx;

    float xpos = vpos.x / g_screen_size_minus_one.x * 5.0f;
    float ypos = ((g_screen_size_minus_one.y - vpos.y) / g_screen_size_minus_one.y) * 10.0f;

    float y_value = norminv_sigma(lerp(0.01f, 5.0f, xpos), 0.7);

    return saturate((y_value * g_screen_size_minus_one.y) - (ypos * g_screen_size_minus_one.y)).xxxx;
}

float3 get_reflectivity_base(in float3 light_vec, in float3 normal_vec, in float3 view_vec, in float3 material_reflectivity, in float smoothness, in float roughness, in float light_vec_reflected_view_vec_angle)
{
    float n_dot_l = dot(light_vec, normal_vec);

    if (n_dot_l <= 0.0f)
        return float3(0, 0, 0);

    float fraction_of_facets = determine_fraction_of_facets_at_reflection_angle(smoothness, light_vec_reflected_view_vec_angle);

    // TODO: REMOVE - DEBUGGIN CODE:
    float facet_visibility = 1.0;

    //float facet_visibility = determine_facet_visibility(roughness, normal_vec, light_vec); // Looks ok

    float3 surface_reflectivity = determine_surface_reflectivity(material_reflectivity, roughness, light_vec, view_vec);

    return fraction_of_facets * facet_visibility * surface_reflectivity;
}

float ensure_correct_trig_value(in float value)
{
    return clamp(value, -1.0f, +1.0f);
}

float3 get_reflectivity_dir_light(in float3 light_vec, in float3 normal_vec, in float3 view_vec, in float3 reflected_view_vec, in float standard_material_reflectivity, in float smoothness, in float roughness)
{
    float light_vec_reflected_view_vec_angle = acos(ensure_correct_trig_value(dot(light_vec, reflected_view_vec)));

    return get_reflectivity_base(light_vec, normal_vec, view_vec, standard_material_reflectivity, smoothness, roughness, light_vec_reflected_view_vec_angle);
}

float3 get_reflectivity_env_light(in float3 light_vec, in float3 normal_vec, in float3 view_vec, in float standard_material_reflectivity, in float smoothness, in float roughness)
{
    return determine_surface_reflectivity(standard_material_reflectivity, roughness, light_vec, view_vec);
}

//float3 standard_lighting_model_directional_light_UPDATED2(in float3 LightColor, in float3 normalised_light_dir, in float3 normalised_view_dir, in StandardLightingModelMaterial_UPDATED material, float envRadiance = 1.0)
//{
//    LightColor *= get_game_hdr_lighting_multiplier();
//    float3 diffuse_scale_factor = get_diffuse_scale_factor();

//    float roughness = 1.0f - material.Smoothness;

//    normalised_light_dir = -normalised_light_dir;
//    normalised_view_dir = normalised_view_dir;

//    float normal_dot_light_vec = max(0.0f, dot(material.Normal, normalised_light_dir));

//    float3 reflected_view_vec = normalize(reflect(normalised_view_dir, material.Normal));

//    float texture_num_lods = cb_reflections_roughness;
//    float env_map_lod = roughness * (texture_num_lods - 1);

//    reflected_view_vec = mul(reflected_view_vec, (float3x3) mEnv); // rotate ENV map
//    float3 environment_colour = get_environment_colour_UPDATED(reflected_view_vec, env_map_lod) * envRadiance;

//     // BEGIN: debugging code
//    return float4(environment_colour, 1); // TODO: REMOVE: DEBUGGIN CODDE
//     // END: debugging code

//    float3 dlight_pixel_reflectivity = get_reflectivity_dir_light(normalised_light_dir, material.Normal, normalised_view_dir, reflected_view_vec, material.Reflectivity, material.Smoothness, roughness);
//    float3 dlight_specular_colour = dlight_pixel_reflectivity * material.Specular_Colour * LightColor;
//    float3 dlight_material_scattering = 1.0f.xxx - max(dlight_pixel_reflectivity, material.Reflectivity.xxx); //  All photons not accounted for by reflectivity are accounted by scattering. From the energy difference between in-coming light and emitted light we could calculate the amount of energy turned into heat. This energy would not be enough to make a viewable difference at standard illumination levels.

//    float3 env_light_pixel_reflectivity = max(material.Reflectivity, get_reflectivity_env_light(reflected_view_vec, material.Normal, normalised_view_dir, material.Reflectivity, material.Smoothness, roughness));
//    float3 env_light_specular_colour = environment_colour * env_light_pixel_reflectivity * material.Specular_Colour;

//    float3 dlight_diffuse = material.Diffuse_Colour * normal_dot_light_vec * LightColor * dlight_material_scattering;

//    float3 ambient_normal = mul(material.Normal, (float3x3) mEnv); // rotate ENV map
//    float3 ambient_colour = cube_ambient(ambient_normal) * envRadiance;

//    // BEGIN: debugging code
//    return float4(dlight_pixel_reflectivity, 1); // TODO: REMOVE: DEBUGGIN CODDE
//    // END: debugging code
//    float3 env_light_diffuse = ambient_colour * material.Diffuse_Colour * (1.0f - material.Reflectivity);

//    dlight_diffuse *= diffuse_scale_factor;

//    //-- BEGIN: debuggin coden
//    return float4(env_light_diffuse.rgb, 1); // TODO: REMOVE DEBUGGING code
//    //-- END: debugging code

//    float shadow_attenuation = material.Shadow;
//    float reflection_shadow_attenuation = (1 - ((1 - (material.Shadow)) * 0.75));
//	//float reflection_shadow_attenuation = material.Shadow;

//    return (material.SSAO * (env_light_diffuse + (reflection_shadow_attenuation * env_light_specular_colour))) + (shadow_attenuation * (dlight_specular_colour + dlight_diffuse));
//}

float3 standard_lighting_model_directional_light_UPDATED(in float3 LightColor, in float3 normalised_light_dir, in float3 normalised_view_dir, in StandardLightingModelMaterial_UPDATED material)
{
    LightColor *= get_game_hdr_lighting_multiplier();
    float3 diffuse_scale_factor = get_diffuse_scale_factor();

    float roughness = 1.0f - material.Smoothness;

    normalised_light_dir = normalised_light_dir;
    normalised_view_dir = -normalised_view_dir;

    float normal_dot_light_vec = max(0.0f, dot(material.Normal, normalised_light_dir));

    float3 reflected_view_vec = reflect(-normalised_view_dir, material.Normal);

    float texture_num_lods = reflections_roughness;

    float env_map_lod = roughness * (texture_num_lods - 1);

    float3 environment_colour = get_environment_colour_UPDATED(reflected_view_vec, env_map_lod);

    // BEGIN: DEBUGGING CODE
    //return float4(environment_colour, 1);
    // END: DEBUGGIN CODE

    float3 dlight_pixel_reflectivity = get_reflectivity_dir_light(normalised_light_dir, material.Normal, normalised_view_dir, reflected_view_vec, material.Reflectivity, material.Smoothness, roughness);
    float3 dlight_specular_colour = dlight_pixel_reflectivity * material.Specular_Colour * LightColor;
    float3 dlight_material_scattering = 1.0f.xxx - max(dlight_pixel_reflectivity, material.Reflectivity.xxx); //  All photons not accounted for by reflectivity are accounted by scattering. From the energy difference between in-coming light and emitted light we could calculate the amount of energy turned into heat. This energy would not be enough to make a viewable difference at standard illumination levels.

    float3 env_light_pixel_reflectivity = max(material.Reflectivity, get_reflectivity_env_light(reflected_view_vec, material.Normal, normalised_view_dir, material.Reflectivity, material.Smoothness, roughness));
    float3 env_light_specular_colour = environment_colour * env_light_pixel_reflectivity * material.Specular_Colour;

    float3 dlight_diffuse = material.Diffuse_Colour * normal_dot_light_vec * LightColor * dlight_material_scattering;

    float3 ambient_colour = cube_ambient(material.Normal);

    float3 env_light_diffuse = ambient_colour * material.Diffuse_Colour * (1.0f - material.Reflectivity);

    dlight_diffuse *= diffuse_scale_factor;

	//if (!b_shadows)
	//{
	//	material.Shadow = 1.0f;
	//}
	//else
	{
        material.Shadow = material.Shadow;
    }
    float shadow_attenuation = material.Shadow;
    float reflection_shadow_attenuation = (1 - ((1 - (material.Shadow)) * 0.75));

    return (material.SSAO * (env_light_diffuse + (reflection_shadow_attenuation * env_light_specular_colour))) + (shadow_attenuation * (dlight_specular_colour + dlight_diffuse));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	Inputs/Outputs
////////////////////////////////////////////////////////////////////////////////////////////////////

struct APP_INPUT
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Bitangent : BINORMAL;
    float4 TexCoord0 : TEXCOORD0;
    float4 TexCoord1 : TEXCOORD1;
    float3 Color : TEXCOORD4;
    float Alpha : TEXCOORD5;
};

struct VS_OUTPUT
{
    float4 Position : POSITION;
    float4 TexCoord : TEXCOORD0;
    float3 I : TEXCOORD1;
    float3 Tgt : TEXCOORD2;
    float3 Btgt : TEXCOORD3;
    float3 Nml : TEXCOORD4;
    float3 Wpos : TEXCOORD5;
    float4 Color : TEXCOORD6;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//	Vertex Shader
////////////////////////////////////////////////////////////////////////////////////////////////////

//VS_OUTPUT vs30_main(in APP_INPUT input)
//{
//    VS_OUTPUT output;

//    output.Position = mul(input.Position, wvpMatrix);
//    output.TexCoord.xy = input.TexCoord0.xy;
//    output.TexCoord.zw = input.TexCoord1.xy;

//    output.TexCoord.y += 1;
//    output.TexCoord.w += 1;

//    output.I = normalize(vMatrix[3] - mul(input.Position, wMatrix));
//    output.Tgt = mul(float4(input.Tangent.xyz, 0.0f), wMatrix).xyz;
//    output.Btgt = mul(float4(input.Bitangent.xyz, 0.0f), wMatrix).xyz;
//    output.Nml = mul(float4(input.Normal.xyz, 0.0f), wMatrix).xyz;
//    output.Wpos = mul(input.Position, wMatrix);
//    output.Color.rgb = input.Color.rgb;
//    output.Color.a = input.Alpha.r;
//    return output;
//}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	Pixel Shader
////////////////////////////////////////////////////////////////////////////////////////////////////

void alpha_test(in const float pixel_alpha)
{
    float texture_alpha_ref = 0.7;

    //if (alpha_used == 1)
    {
        clip(pixel_alpha - texture_alpha_ref);
    }
}

//float4 ps30_main_UPDATED(in VS_OUTPUT input) : COLOR
//{
//    float3 eye_vector = -normalize(vMatrixI[3] - input.Wpos);

//    float3 light_vector = normalize(light_position0.xyz - input.Wpos);

//    float4 diffuse_colour = tex2D(s_diffuse_colour, input.TexCoord.xy);

//    alpha_test(diffuse_colour.a);

//    float4 specular_colour = tex2D(s_specular_colour, input.TexCoord.xy);

//    float smoothness = tex2D(s_smoothness, input.TexCoord.xy).x;

//    float reflectivity = tex2D(s_reflectivity, input.TexCoord.xy).x;

//    float mask_p1 = tex2D(s_mask1, input.TexCoord.xy).r;
//    float mask_p2 = tex2D(s_mask2, input.TexCoord.xy).r;
//    float mask_p3 = tex2D(s_mask3, input.TexCoord.xy).r;

//    if (b_faction_colouring)
//    {
//        diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * get_adjusted_faction_colour(_linear(vec4_colour_0.rgb)), mask_p1);
//        diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * get_adjusted_faction_colour(_linear(vec4_colour_1.rgb)), mask_p2);
//        diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * get_adjusted_faction_colour(_linear(vec4_colour_2.rgb)), mask_p3);
//    }
//    else
//    {
//        diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * _linear(vec4_colour_0.rgb), mask_p1);
//        diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * _linear(vec4_colour_1.rgb), mask_p2);
//        diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * _linear(vec4_colour_2.rgb), mask_p3);
//    }

//    float3x3 basis = MAXTBN

//    float3 Np = (tex2D(s_normal_map, input.TexCoord.xy)).rgb;
//    float3 N = normalSwizzle_UPDATED((Np.rgb * 2.0f) - 1.0f);

//    float3 pixel_normal = normalize(mul(normalize(N), basis));

//    StandardLightingModelMaterial_UPDATED standard_mat = create_standard_lighting_material_UPDATED(diffuse_colour, specular_colour, pixel_normal, smoothness, reflectivity, float4(input.Wpos.xyz, 1));

//    float3 hdr_linear_col = standard_lighting_model_directional_light_UPDATED(light_color0, light_vector, eye_vector, standard_mat);

//    float3 ldr_linear_col = saturate(tone_map_linear_hdr_pixel_value(hdr_linear_col));

//    return float4(_gamma(ldr_linear_col), 1.0f);
//}

//float4 ps30_main_decaldirt(in VS_OUTPUT input) : COLOR
//{
//    float3 eye_vector = -normalize(vMatrixI[3] - input.Wpos);

//    float3 light_vector = normalize(light_position0.xyz - input.Wpos);

//    float4 diffuse_colour = tex2D(s_diffuse_colour, input.TexCoord.xy);

//    alpha_test(diffuse_colour.a);

//    float4 specular_colour = tex2D(s_specular_colour, input.TexCoord.xy);

//    float smoothness = tex2D(s_smoothness, input.TexCoord.xy).x;

//    float reflectivity = tex2D(s_reflectivity, input.TexCoord.xy).x;

//    float mask_p1 = tex2D(s_mask1, input.TexCoord.xy).r;
//    float mask_p2 = tex2D(s_mask2, input.TexCoord.xy).r;
//    float mask_p3 = tex2D(s_mask3, input.TexCoord.xy).r;

//    diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * _linear(vec4_colour_0.rgb), mask_p1);
//    diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * _linear(vec4_colour_1.rgb), mask_p2);
//    diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * _linear(vec4_colour_2.rgb), mask_p3);

//    float3x3 basis = MAXTBN

//    float3 Np = (tex2D(s_normal_map, input.TexCoord.xy)).rgb;
//    float3 N = normalSwizzle_UPDATED((Np.rgb * 2.0f) - 1.0f);

//    if (b_do_decal)
//    {
//        ps_common_blend_decal(diffuse_colour, N, specular_colour.rgb, reflectivity, diffuse_colour, N, specular_colour.rgb, reflectivity, input.TexCoord.xy, 0, vec4_uv_rect, 1.0);
//    }

//    if (b_do_dirt)
//    {
//        ps_common_blend_dirtmap(diffuse_colour, N, specular_colour.rgb, reflectivity, diffuse_colour, N, specular_colour.rgb, reflectivity, input.TexCoord.xy, float2(f_uv_offset_u, f_uv_offset_v));
//    }

//    float3 pixel_normal = normalize(mul(normalize(N), basis));

//    StandardLightingModelMaterial_UPDATED standard_mat = create_standard_lighting_material_UPDATED(diffuse_colour, specular_colour, pixel_normal, smoothness, reflectivity, float4(input.Wpos.xyz, 1));

//    float3 hdr_linear_col = standard_lighting_model_directional_light_UPDATED(light_color0, light_vector, eye_vector, standard_mat);

//    float3 ldr_linear_col = saturate(tone_map_linear_hdr_pixel_value(hdr_linear_col));

//    return float4(_gamma(ldr_linear_col), 1);
//}

//float4 ps30_main_custom_terrain(in VS_OUTPUT input) : COLOR
//{
//    float3 eye_vector = -normalize(vMatrixI[3] - input.Wpos);

//    float3 light_vector = normalize(light_position0.xyz - input.Wpos);

//    float4 diffuse_colour = tex2D(s_diffuse_colour, input.TexCoord.xy);

//    alpha_test(diffuse_colour.a);

//    float4 specular_colour = tex2D(s_specular_colour, input.TexCoord.xy);

//    float smoothness = tex2D(s_smoothness, input.TexCoord.xy).x;

//    float reflectivity = tex2D(s_reflectivity, input.TexCoord.xy).x;

//    float3x3 basis = MAXTBN

//    float3 Np = (tex2D(s_normal_map, input.TexCoord.xy)).rgb;
//    float3 N = normalSwizzle_UPDATED((Np.rgb * 2.0f) - 1.0f);

//    ps_common_blend_decal(diffuse_colour, N, specular_colour.rgb, reflectivity, diffuse_colour, N, specular_colour.rgb, reflectivity, input.TexCoord.xy, 0, vec4_uv_rect, 1 - input.Color.a);

//    float3 pixel_normal = normalize(mul(normalize(N), basis));

//    StandardLightingModelMaterial_UPDATED standard_mat = create_standard_lighting_material_UPDATED(diffuse_colour, specular_colour, pixel_normal, smoothness, reflectivity, float4(input.Wpos.xyz, 1));

//    float3 hdr_linear_col = standard_lighting_model_directional_light_UPDATED(light_color0, light_vector, eye_vector, standard_mat);

//    float3 ldr_linear_col = saturate(tone_map_linear_hdr_pixel_value(hdr_linear_col));

//    return float4(_gamma(ldr_linear_col), 1);
//}

//float4 ps30_main_vfx(in VS_OUTPUT input) : COLOR
//{
//    float3 eye_vector = -normalize(vMatrixI[3] - input.Wpos);

//    float3 light_vector = normalize(light_position0.xyz - input.Wpos);

//    float4 diffuse_colour = tex2D(s_diffuse_colour, input.TexCoord.xy);

//    alpha_test(diffuse_colour.a);

//    float4 specular_colour = tex2D(s_specular_colour, input.TexCoord.xy);

//    float smoothness = tex2D(s_smoothness, input.TexCoord.xy).x;

//    float reflectivity = tex2D(s_reflectivity, input.TexCoord.xy).x;

//    float mask_p1 = tex2D(s_mask1, input.TexCoord.xy).r;
//    float mask_p2 = tex2D(s_mask2, input.TexCoord.xy).r;
//    float mask_p3 = tex2D(s_mask3, input.TexCoord.xy).r;

//    diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * _linear(vec4_colour_0.rgb), mask_p1);
//    diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * _linear(vec4_colour_1.rgb), mask_p2);
//    diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * _linear(vec4_colour_2.rgb), mask_p3);

//    float3x3 basis = MAXTBN

//    float3 Np = (tex2D(s_normal_map, input.TexCoord.xy)).rgb;
//    float3 N = normalSwizzle_UPDATED((Np.rgb * 2.0f) - 1.0f);

//    if (b_do_decal)
//    {
//        ps_common_blend_decal(diffuse_colour, N, specular_colour.rgb, reflectivity, diffuse_colour, N, specular_colour.rgb, reflectivity, input.TexCoord.xy, 0, vec4_uv_rect, 1.0);
//    }

//    if (b_do_dirt)
//    {
//        ps_common_blend_dirtmap(diffuse_colour, N, specular_colour.rgb, reflectivity, diffuse_colour, N, specular_colour.rgb, reflectivity, input.TexCoord.xy, float2(f_uv_offset_u, f_uv_offset_v));
//    }

//    ps_common_blend_vfx(diffuse_colour, N, specular_colour.rgb, reflectivity, diffuse_colour, N, specular_colour.rgb, reflectivity, input.TexCoord.xy, float2(f_uv_offset_u, f_uv_offset_v));

//    float3 pixel_normal = normalize(mul(normalize(N), basis));

//    StandardLightingModelMaterial_UPDATED standard_mat = create_standard_lighting_material_UPDATED(diffuse_colour, specular_colour, pixel_normal, smoothness, reflectivity, float4(input.Wpos.xyz, 1));

//    float3 hdr_linear_col = standard_lighting_model_directional_light_UPDATED(light_color0, light_vector, eye_vector, standard_mat);

//    float3 ldr_linear_col = saturate(tone_map_linear_hdr_pixel_value(hdr_linear_col));

//    return float4(_gamma(ldr_linear_col), 1);
//}

///////////////////////////////////////////////////////////////
////  TONE MAPPER
///////////////////////////////////////////////////////////////

//	Tone mapping parameters
float Tone_Map_Black = 0.001;
float Tone_Map_White = 10.0f;
float low_tones_scurve_bias = 0.33f;
float high_tones_scurve_bias = 0.66f;

float3 tone_map_linear_hdr_pixel_value(in float3 linear_hdr_pixel_val)
{
    float4 hdr_CIE_LogYxy_pixel = HDR_RGB_To_HDR_CIE_Log_Y_xy(linear_hdr_pixel_val);

    float4 tone_mapped_ldr_CIE_Yxy_pixel = tone_map_HDR_CIE_Log_Y_xy_To_LDR_CIE_Yxy(hdr_CIE_LogYxy_pixel);

    float4 tone_mapped_ldr_linear_rgb = LDR_CIE_Yxy_To_Linear_LDR_RGB(tone_mapped_ldr_CIE_Yxy_pixel);

    return tone_mapped_ldr_linear_rgb.rgb;
}

float4 HDR_RGB_To_HDR_CIE_Log_Y_xy(in float3 linear_colour_val)
{
    float3x3 cie_transform_mat =
    {
        0.4124f, 0.3576f, 0.1805f,
										0.2126f, 0.7152f, 0.0722f,
										0.0193f, 0.1192f, 0.9505f
    };

    float3 cie_XYZ = mul(cie_transform_mat, linear_colour_val);

    float denominator = cie_XYZ.x + cie_XYZ.y + cie_XYZ.z;

    float x = cie_XYZ.x / max(denominator, real_approx_zero);
    float y = cie_XYZ.y / max(denominator, real_approx_zero);

    return float4(log10(max(cie_XYZ.y, real_approx_zero)), x, y, cie_XYZ.y);
}

float4 tone_map_HDR_CIE_Log_Y_xy_To_LDR_CIE_Yxy(in float4 hdr_LogYxy)
{
    float black_point = Tone_Map_Black;
    float white_point = Tone_Map_White;
    float log_Y_black_point = log10(Tone_Map_Black);
    float log_Y_white_point = log10(Tone_Map_White);

    hdr_LogYxy.x = max(hdr_LogYxy.x, log_Y_black_point);

    float log_y_display_range = log_Y_white_point - log_Y_black_point;

    float log_y_in_white_black = (hdr_LogYxy.x - log_Y_black_point) / log_y_display_range;

    float log_y_in_white_black_scurve_biased = get_scurve_y_pos(log_y_in_white_black);

    float biased_log_y = log_Y_black_point + (log_y_in_white_black_scurve_biased * log_y_display_range);

    float biased_y = pow(10.0f, biased_log_y);

    float ldr_y = (biased_y - black_point) / (white_point - black_point);

    return float4(ldr_y, hdr_LogYxy.yzw);
}

float4 LDR_CIE_Yxy_To_Linear_LDR_RGB(in float4 ldr_cie_Yxy)
{
    float Y = ldr_cie_Yxy[0];
    float x = ldr_cie_Yxy[1];
    float y = ldr_cie_Yxy[2];

    float safe_denominator = max(y, real_approx_zero);

    float cie_Y = Y;

    float3 cie_XYZ = { x * cie_Y / safe_denominator, cie_Y, (1 - x - y) * cie_Y / safe_denominator };

    float3x3 cie_XYZ_toRGB_transform_mat =
    {
        +3.2405f, -1.5372f, -0.4985f,
										            -0.9693f, +1.8760f, +0.0416f,
										            +0.0556f, -0.2040f, +1.0572f
    };

    float3 rgb = mul(cie_XYZ_toRGB_transform_mat, cie_XYZ);

    rgb.xyz = max(float3(0, 0, 0), rgb);

    return float4(rgb.xyz, 1.0f);
}

float get_scurve_y_pos(const float x_coord)
{
    float point0_y = 0.0f;
    float point1_y = low_tones_scurve_bias;
    float point2_y = high_tones_scurve_bias;
    float point3_y = 1.0f;

    float4 t = { x_coord * x_coord * x_coord, x_coord * x_coord, x_coord, 1.0f };

    float4x4 BASIS =
    {
        -1.0f, +3.0f, -3.0f, +1.0f,
        			    +3.0f, -6.0f, +3.0f, +0.0f,
        			    -3.0f, +3.0f, +0.0f, +0.0f,
                        +1.0f, +0.0f, +0.0f, +0.0f
    };

    float4 g = mul(t, BASIS);

    return (point0_y * g.x) + (point1_y * g.y) + (point2_y * g.z) + (point3_y * g.w);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	Technique pixel shaders
////////////////////////////////////////////////////////////////////////////////////////////////////

//float4 ps30_flatdiffuse(in VS_OUTPUT input) : COLOR
//{
//    float4 Ct = tex2D(s_diffuse_colour, input.TexCoord.xy);

//    alpha_test(Ct.a);

//    return float4(_gamma(Ct.rgb), 1);
//}

//float4 ps30_albedo(in VS_OUTPUT input) : COLOR
//{
//    float4 Ct = tex2D(s_diffuse_colour, input.TexCoord.xy);

//    alpha_test(Ct.a);

//    return float4(_gamma(Ct.rgb), 1);
//}

//float4 ps30_solidalpha(in VS_OUTPUT input) : COLOR
//{
//    float4 Ct = tex2D(s_diffuse_colour, input.TexCoord.xy);

//    return float4((Ct.aaa), 1.0f);
//}

//float4 ps30_t_roughness(in VS_OUTPUT input) : COLOR
//{
//    float4 roughness_p = tex2D(s_smoothness, input.TexCoord.xy);
//    return float4(_gamma(roughness_p.rrr), 1.0f);
//}

//float4 ps30_t_reflectivity(in VS_OUTPUT input) : COLOR
//{
//    float4 reflectivity_p = tex2D(s_reflectivity, input.TexCoord.xy);
//    return float4(_gamma(reflectivity_p.rrr), 1.0f);
//}

//float4 ps30_t_specular(in VS_OUTPUT input) : COLOR
//{
//    float4 specular_p = tex2D(s_specular_colour, input.TexCoord.xy);
//    return float4(_gamma(specular_p.rgb), 1.0f);
//}

//float4 ps30_t_ao(in VS_OUTPUT input) : COLOR
//{
//    float3 ao = tex2D(s_ambient_occlusion, input.TexCoord.zw);
//    return float4(ao.rgb, 1.0f);
//}

//float4 ps30_t_normal(in VS_OUTPUT input) : COLOR
//{
//    float3 N = normalSwizzle(tex2D(s_normal_map, input.TexCoord.xy).rgb);

//    return float4(N.rgb, 1.0f);
//}

//float4 ps30_t_mask1(in VS_OUTPUT input) : COLOR
//{
//    float4 faction_p = tex2D(s_mask1, input.TexCoord.xy);
//    return float4((faction_p.rrr), 1.0f);
//}

//float4 ps30_t_mask2(in VS_OUTPUT input) : COLOR
//{
//    float4 faction_p = tex2D(s_mask2, input.TexCoord.xy);
//    return float4((faction_p.rrr), 1.0f);
//}

//float4 ps30_t_mask3(in VS_OUTPUT input) : COLOR
//{
//    float4 faction_p = tex2D(s_mask3, input.TexCoord.xy);
//    return float4((faction_p.rrr), 1.0f);
//}

//float4 ps30_t_cubemap(in VS_OUTPUT input) : COLOR
//{
//    float3 pI = normalize(vMatrixI[3] - input.Wpos);
//    const float3x3 basis = MAXTBN

//    float3 N = normalSwizzle(tex2D(s_normal_map, input.TexCoord.xy).rgb * 2.0f - 1.0f);

//    if (1.0)
//    {
//        float3 N2 = normalSwizzle(tex2D(s_d_normal, input.TexCoord.xy * 1.0).rgb * 2.0f - 1.0f);
//        N = float3(N.x + (N2.x * 1.0), N.y + (N2.y * 1.0), N.z);
//    }

//    float3 nN = normalize(mul(N, basis));
//    float3 refl = reflect(pI, nN);
//    refl.z = -refl.z;
//    float3 env = get_environment_colour_UPDATED(refl, 0.0);

//    return float4(_gamma(env.rgb), 1.0f);
//}

//float4 ps30_t_ambient(in VS_OUTPUT input) : COLOR
//{
//    const float3x3 basis = MAXTBN

//    float3 N = normalSwizzle(tex2D(s_normal_map, input.TexCoord.xy).rgb * 2.0f - 1.0f);

//    if (1.0)
//    {
//        float3 N2 = normalSwizzle(tex2D(s_d_normal, input.TexCoord.xy * 1.0).rgb * 2.0f - 1.0f);
//        N = float3(N.x + (N2.x * 1.0), N.y + (N2.y * 1.0), N.z);
//    }

//    float3 nN = normalize(mul(N, basis));

//    return float4(_gamma(cube_ambient(nN).rgb), 1.0f);
//}

//float4 ps30_t_ws_normal_map(in VS_OUTPUT input) : COLOR
//{
//    const float3x3 basis = MAXTBN

//    float3 N = normalSwizzle(tex2D(s_normal_map, input.TexCoord.xy).rgb * 2.0f - 1.0f);

//    if (1.0)
//    {
//        float3 N2 = normalSwizzle(tex2D(s_d_normal, input.TexCoord.xy * 1.0).rgb * 2.0f - 1.0f);
//        N = float3(N.x + (N2.x * 1.0), N.y + (N2.y * 1.0), N.z);
//    }

//    float3 nN = ((normalize(mul(N, basis))) * 0.5) + 0.5;
//    return float4(nN.rgb, 1.0f);
//}

//float4 ps30_t_dirtmap(in VS_OUTPUT input) : COLOR
//{
//    float2 dirt_scale = float2(f_uv2_tile_interval_u, f_uv2_tile_interval_v);
//    float4 dirtmap_p = tex2D(s_dirtmap_uv2, input.TexCoord.xy * dirt_scale);
//    float4 dirtmap_alpha_p = tex2D(s_alpha_mask, input.TexCoord.zw);

//    return float4(dirtmap_p.rgb, 1.0f);
//}

//float4 ps30_t_alpha_uv2(in VS_OUTPUT input) : COLOR
//{
//    float4 dirtmap_alpha_p = tex2D(s_alpha_mask, input.TexCoord.zw);

//    return float4(dirtmap_alpha_p.rgb, 1.0f);
//}

//float4 ps30_valpha(in VS_OUTPUT input) : COLOR
//{
//    float4 Ct = tex2D(s_diffuse_colour, input.TexCoord.xy);
//    return float4(input.Color.aaa, Ct.a);
//}

//float4 ps30_vcolour(in VS_OUTPUT input) : COLOR
//{
//    float4 Ct = tex2D(s_diffuse_colour, input.TexCoord.xy);

//    return float4(input.Color.rgb, Ct.a);
//}

//float4 ps30_full_tint_UPDATED(in VS_OUTPUT input) : COLOR
//{
//    float3 eye_vector = -normalize(vMatrixI[3] - input.Wpos);

//    float3 light_vector = normalize(light_position0.xyz - input.Wpos);

//    float4 diffuse_colour = tex2D(s_diffuse_colour, input.TexCoord.xy);

//    alpha_test(diffuse_colour.a);

//    float4 specular_colour = tex2D(s_specular_colour, input.TexCoord.xy);

//    float smoothness = tex2D(s_smoothness, input.TexCoord.xy).x;

//    float reflectivity = tex2D(s_reflectivity, input.TexCoord.xy).x;

//    float3 ao = tex2D(s_ambient_occlusion, input.TexCoord.zw).rgb;
//    float mask_p1 = tex2D(s_mask1, input.TexCoord.xy).r;
//    float mask_p2 = tex2D(s_mask2, input.TexCoord.xy).r;
//    float mask_p3 = tex2D(s_mask3, input.TexCoord.xy).r;

//    if (b_faction_colouring)
//    {
//        diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * _linear(vec4_colour_0.rgb), mask_p1);
//        diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * _linear(vec4_colour_1.rgb), mask_p2);
//        diffuse_colour.rgb = lerp(diffuse_colour.rgb, diffuse_colour.rgb * _linear(vec4_colour_2.rgb), mask_p3);
//    }

//    float3x3 basis = MAXTBN

//    float4 Np = (tex2D(s_normal_map, input.TexCoord.xy));
//    float3 N = normalSwizzle_UPDATED((Np.rgb * 2.0f) - 1.0f);
//    float3 pixel_normal = normalize(mul(normalize(N), basis));

//    StandardLightingModelMaterial_UPDATED standard_mat = create_standard_lighting_material_UPDATED(diffuse_colour, specular_colour, pixel_normal, smoothness, reflectivity, float4(input.Wpos.xyz, 1));

//    float3 hdr_linear_col = standard_lighting_model_directional_light_UPDATED(light_color0, light_vector, eye_vector, standard_mat);

//    float3 ldr_linear_col = saturate(tone_map_linear_hdr_pixel_value(hdr_linear_col));

//    return float4(_gamma(ldr_linear_col), 1.0f);
//}

//float4 ps30_full_ao(in VS_OUTPUT input) : COLOR
//{
//    float3 eye_vector = -normalize(vMatrixI[3] - input.Wpos);

//    float3 light_vector = normalize(light_position0.xyz - input.Wpos);

//    float4 diffuse_colour = tex2D(s_diffuse_colour, input.TexCoord.xy);

//    alpha_test(diffuse_colour.a);

//    float4 specular_colour = tex2D(s_specular_colour, input.TexCoord.xy);

//    float smoothness = tex2D(s_smoothness, input.TexCoord.xy).x;

//    float reflectivity = tex2D(s_reflectivity, input.TexCoord.xy).x;

//    float3 ao = tex2D(s_ambient_occlusion, input.TexCoord.zw);
//    diffuse_colour.rgb *= ao.rgb;
//    specular_colour.rgb *= ao.rgb;

//    float3x3 basis = MAXTBN

//    float4 Np = (tex2D(s_normal_map, input.TexCoord.xy));
//    float3 N = normalSwizzle_UPDATED((Np.rgb * 2.0f) - 1.0f);
//    float3 pixel_normal = normalize(mul(normalize(N), basis));

//    StandardLightingModelMaterial_UPDATED standard_mat = create_standard_lighting_material_UPDATED(diffuse_colour, specular_colour, pixel_normal, smoothness, reflectivity, float4(input.Wpos.xyz, 1));

//    float3 hdr_linear_col = standard_lighting_model_directional_light_UPDATED(light_color0, light_vector, eye_vector, standard_mat);

//    float3 ldr_linear_col = saturate(tone_map_linear_hdr_pixel_value(hdr_linear_col));

//    return float4(_gamma(ldr_linear_col), 1.0f);
//}

//float4 ps30_full_dirtmap(in VS_OUTPUT input) : COLOR
//{
//    float3 eye_vector = -normalize(vMatrixI[3] - input.Wpos);

//    float3 light_vector = normalize(light_position0.xyz - input.Wpos);

//    float4 diffuse_colour = tex2D(s_diffuse_colour, input.TexCoord.xy);

//    alpha_test(diffuse_colour.a);

//    float4 specular_colour = tex2D(s_specular_colour, input.TexCoord.xy);

//    float smoothness = tex2D(s_smoothness, input.TexCoord.xy).x;

//    float reflectivity = tex2D(s_reflectivity, input.TexCoord.xy).x;

//    float4 dirtmap = tex2D(s_dirtmap_uv2, input.TexCoord.xy * float2(f_uv2_tile_interval_u, f_uv2_tile_interval_v));
//    float alpha_mask = tex2D(s_alpha_mask, input.TexCoord.zw);

//    float blend_amount = alpha_mask;

//    float hardness = 1.0;

//    float blend_2 = blend_amount * lerp(1, dirtmap.a, blend_amount);

//    blend_amount = saturate(((blend_2 - 0.5) * hardness) + 0.5);

//    diffuse_colour.rgb = diffuse_colour.rgb * (lerp(dirtmap.rgb, float3(1, 1, 1), blend_amount));
//    specular_colour.rgb *= (lerp(dirtmap.rgb, float3(1, 1, 1), blend_amount));

//    float3x3 basis = MAXTBN

//    float4 Np = (tex2D(s_normal_map, input.TexCoord.xy));
//    float3 N = normalSwizzle_UPDATED((Np.rgb * 2.0f) - 1.0f);
//    float3 pixel_normal = normalize(mul(normalize(N), basis));

//    StandardLightingModelMaterial_UPDATED standard_mat = create_standard_lighting_material_UPDATED(diffuse_colour, specular_colour, pixel_normal, smoothness, reflectivity, float4(input.Wpos.xyz, 1));

//    float3 hdr_linear_col = standard_lighting_model_directional_light_UPDATED(light_color0, light_vector, eye_vector, standard_mat);

//    float3 ldr_linear_col = saturate(tone_map_linear_hdr_pixel_value(hdr_linear_col));

//    return float4(_gamma(ldr_linear_col), 1.0f);
//}

//float4 ps30_full_skin(in VS_OUTPUT input) : COLOR
//{
//    float3 eye_vector = -normalize(vMatrixI[3] - input.Wpos);

//    float3 light_vector = normalize(light_position0.xyz - input.Wpos);

//    float4 diffuse_colour = tex2D(s_diffuse_colour, input.TexCoord.xy);

//    alpha_test(diffuse_colour.a);

//    float4 specular_colour = tex2D(s_specular_colour, input.TexCoord.xy);

//    float smoothness = tex2D(s_smoothness, input.TexCoord.xy).x;

//    float reflectivity = tex2D(s_reflectivity, input.TexCoord.xy).x;

//    float3 ao = tex2D(s_ambient_occlusion, input.TexCoord.zw).rgb;
//    float mask_p1 = tex2D(s_mask1, input.TexCoord.xy).r;
//    float mask_p2 = tex2D(s_mask2, input.TexCoord.xy).r;
//    float mask_p3 = tex2D(s_mask3, input.TexCoord.xy).r;

//    float3x3 basis = MAXTBN

//    float4 Np = (tex2D(s_normal_map, input.TexCoord.xy));
//    float3 N = normalSwizzle_UPDATED((Np.rgb * 2.0f) - 1.0f);

//    if (b_do_decal)
//    {
//        ps_common_blend_decal(diffuse_colour, N, specular_colour.rgb, reflectivity, diffuse_colour, N, specular_colour.rgb, reflectivity, input.TexCoord.xy, 0, vec4_uv_rect, 1.0);
//    }

//    if (b_do_dirt)
//    {
//        ps_common_blend_dirtmap(diffuse_colour, N, specular_colour.rgb, reflectivity, diffuse_colour, N, specular_colour.rgb, reflectivity, input.TexCoord.xy, float2(f_uv_offset_u, f_uv_offset_v));
//    }

//    float3 pixel_normal = normalize(mul(normalize(N), basis));

//    SkinLightingModelMaterial skin_mat = create_skin_lighting_material(float2(smoothness, reflectivity), float3(mask_p1, mask_p2, mask_p3), diffuse_colour, specular_colour, pixel_normal, float4(input.Wpos.xyz, 1));

//    float3 hdr_linear_col = skin_lighting_model_directional_light(light_color0, light_vector, eye_vector, skin_mat);

//    float3 ldr_linear_col = saturate(tone_map_linear_hdr_pixel_value(hdr_linear_col));

//    return float4(_gamma(ldr_linear_col), 1.0);
//}

//float4 ps30_ndotl(in VS_OUTPUT input) : COLOR
//{
//    float4 diffuse_colour = tex2D(s_diffuse_colour, input.TexCoord.xy);
//    float3 light_vector = normalize(light_position0.xyz - input.Wpos);

//    float3x3 basis = MAXTBN

//    float4 Np = (tex2D(s_normal_map, input.TexCoord.xy));
//    float3 N = normalSwizzle_UPDATED((Np.rgb * 2.0f) - 1.0f);
//    float3 pixel_normal = normalize(mul(normalize(N), basis));

//    float3 ndotl = saturate(dot(pixel_normal, light_vector));

//    return float4(_gamma(ndotl), diffuse_colour.a);
//}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	Techniques
////////////////////////////////////////////////////////////////////////////////////////////////////