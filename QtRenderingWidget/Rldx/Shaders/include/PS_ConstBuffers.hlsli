#define NUMBER_OF_LIGHTS 1

struct LightSourceData
{
    float3 lightDirection;
    float radiannce;
    float4 lightColor;
};

cbuffer PS_LightBuffer : register(b0)
{
    LightSourceData LightData[NUMBER_OF_LIGHTS];
    float4 env_color;
    float4x4 mEnv;
    
    float4 solidColor;
    
    float smoothness_curve;
    float reflect_curve;
    float reflections_roughness;
    float shadow_factor;

    float env_light_radiance;
    uint use_AO;
    uint all_transparent;
    float exposure;

    float bloom_threshold;
    float bloom_intensity;
    int maskIndex;
    float reserved___PS_LightBuffer_2;

    float4 screenRes;
    	                 	
    /*
        color1, color2, color3 in cpu buffer
    */
    float4 colorTable[3];
};

//cbuffer PS_PerFrame_ConstBuffer : register(b0)
//{
//    float4x4 rotEnv;

//    float3 lightDirection;
//    float radiannce;

//    float4 ambientLight;
//    float4 diffuseColor;
//    float4 specularColor;

//    float ambientFactor;
//    float diffuseFactor;
//    float specularFactor;
//    float roughnessFactor;

//}

cbuffer PS_PerMesh_ConstBuffer : register(b1)
{    
    float reflectivity;
    float ambientlight;
    float reserved1;
    float reserved2;

    uint bDoFactionColoring;
    uint bDoTinting;
    uint alpha_used;
    uint mask_index;
	
    float decal_u;
    float decal_v;
    float u_div;
    float v_div;
	//float reserved5;	

    float4 decal_uv_rect;

    float4 color_no_tex;

    float4 mesh_faction_color1;
    float4 mesh_faction_color2;
    float4 mesh_faction_color3;

    float4 tint_color1;
    float4 tint_color2;
    float4 tint_color3;   

    uint4 puiMaskIndices = { 0, 1, 2, 0 };

    uint PisTextureSRGB[64];
};

//cbuffer PS_ConstBuffer_PerIndivual_Mesh : register(b2)
//{
//    struct _PS_Mesh
//    {    
//        float reserved1;
//        float reflectivity;
//        float ambientlight;

//        uint bDoFactionColoring;

//        uint alpha_used;
//        uint mask_index;
//        float decal_u;
//        float decal_v;

//        float u_div;
//        float v_div;
//        float reserved5;
//        float reserved6;

//        float4 decal_uv_rect; // TODO: is this value for statics only?

//        float4 color_no_tex;
//        float4 color1;
//        float4 color2;
//        float4 color3;

//        float4 tint_color1;
//        float4 tint_color2;
//        float4 tint_color3;

//        uint4 puiMaskIndices;

//        uint PisTextureSRGB[64];
//    } PS_Mesh;
//};



//cbuffer PS_PerModel_ConstBuffer_TESTING : register(b3)
//{
//    float4 color_test;
//}

//#define NUMBER_OF_LIGHTS 1

//struct LightSourceData
//{
//    float3 lightDirection;
//    float radiannce;
//    float4 lightColor;
//};

//cbuffer PS_LightBuffer : register(b0)
//{
//    LightSourceData LightData[NUMBER_OF_LIGHTS];
//    float4 env_color;
//    float4x4 mEnv;

//    float smoothness_curve;
//    float reflect_curve;
//    float cb_reflections_roughness;
//    float reserved4;

//    float env_light_radiance;
//    uint use_AO;
//    uint all_transparent;
//    float exposure;

//    float4 sreeenRes;
//}

////cbuffer PS_PerFrame_ConstBuffer : register(b0)
////{
////    float4x4 rotEnv;

////    float3 lightDirection;
////    float radiannce;

////    float4 ambientLight;
////    float4 diffuseColor;
////    float4 specularColor;

////    float ambientFactor;
////    float diffuseFactor;
////    float specularFactor;
////    float roughnessFactor;

////}

//cbuffer PS_PerModel_ConstBuffer : register(b1)
//{

//    float reserved1;
//    float reflectivity;
//    float ambientlight;
//    float reserved2;

//    uint alpha_used;
//    uint render_fresnel;
//    float u_div;
//    float v_div;

//    float4 decal_uv_rect; // TODO: is this value for statics only?

//    float4 color_no_tex;
//    float4 color1;
//    float4 color2;
//    float4 color3;

//    uint PisTextureSRGB[64];

//}

//#define NUMBER_OF_LIGHTS 1

//struct LightSourceData
//{
//    float3 lightDirection;
//    float radiannce;
//    float4 lightColor;
//};

//cbuffer PS_LightBuffer : register(b0)
//{
//    LightSourceData LightData[NUMBER_OF_LIGHTS];
//    float4 env_color;
//    float4x4 mEnv;
    
//    float4 solidColor;
    
//    float smoothness_curve;
//    float reflect_curve;
//    float cb_reflections_roughness;
//    float shadow_factor;

//    float env_light_radiance;
//    uint use_AO;
//    uint all_transparent;
//    float exposure;

//    float bloom_threshold;
//    float bloom_intensity;
//    float reserved___PS_LightBuffer_1;
//    float reserved___PS_LightBuffer_2;   

//    float4 screenRes;
//}

////cbuffer PS_PerFrame_ConstBuffer : register(b0)
////{
////    float4x4 rotEnv;

////    float3 lightDirection;
////    float radiannce;

////    float4 ambientLight;
////    float4 diffuseColor;
////    float4 specularColor;

////    float ambientFactor;
////    float diffuseFactor;
////    float specularFactor;
////    float roughnessFactor;

////}

//cbuffer PS_PerMesh_ConstBuffer : register(b1)
//{
//    float decal_u;
//    float decal_v;
//    float u_div;
//    float v_div;
	
//    float4 v4DecalUVRectangle;  // float4 value to do with (ONLY static?) decals, maybe?

//    float reflectivity = 1.0;
//    float ambientlight = 1.0;
//    float reserved1;
//    float reserved2;

//    uint bDoFactionColoring = 1;
//    uint bDoTinting = 1;
//    uint alpha_used = 1;
//    uint mask_index = 0;
    
//    float4 color_no_tex;

//    float4 color1;
//    float4 color2;
//    float4 color3;

//    float4 tint_color1;
//    float4 tint_color2;
//    float4 tint_color3;

//    uint4 puiMaskIndices;

//    uint PisTextureSRGB[64];
//};

////cbuffer PS_ConstBuffer_PerIndivual_Mesh : register(b2)
////{
////    struct _PS_Mesh
////    {    
////        float reserved1;
////        float reflectivity;
////        float ambientlight;

////        uint bDoFactionColoring;

////        uint alpha_used;
////        uint mask_index;
////        float decal_u;
////        float decal_v;

////        float u_div;
////        float v_div;
////        float reserved5;
////        float reserved6;

////        float4 decal_uv_rect; // TODO: is this value for statics only?

////        float4 color_no_tex;
////        float4 color1;
////        float4 color2;
////        float4 color3;

////        float4 tint_color1;
////        float4 tint_color2;
////        float4 tint_color3;

////        uint4 puiMaskIndices;

////        uint PisTextureSRGB[64];
////    } PS_Mesh;
////};



////cbuffer PS_PerModel_ConstBuffer_TESTING : register(b3)
////{
////    float4 color_test;
////}

////#define NUMBER_OF_LIGHTS 1

////struct LightSourceData
////{
////    float3 lightDirection;
////    float radiannce;
////    float4 lightColor;
////};

////cbuffer PS_LightBuffer : register(b0)
////{
////    LightSourceData LightData[NUMBER_OF_LIGHTS];
////    float4 env_color;
////    float4x4 mEnv;

////    float smoothness_curve;
////    float reflect_curve;
////    float cb_reflections_roughness;
////    float reserved4;

////    float env_light_radiance;
////    uint use_AO;
////    uint all_transparent;
////    float exposure;

////    float4 sreeenRes;
////}

//////cbuffer PS_PerFrame_ConstBuffer : register(b0)
//////{
//////    float4x4 rotEnv;

//////    float3 lightDirection;
//////    float radiannce;

//////    float4 ambientLight;
//////    float4 diffuseColor;
//////    float4 specularColor;

//////    float ambientFactor;
//////    float diffuseFactor;
//////    float specularFactor;
//////    float roughnessFactor;

//////}

////cbuffer PS_PerModel_ConstBuffer : register(b1)
////{

////    float reserved1;
////    float reflectivity;
////    float ambientlight;
////    float reserved2;

////    uint alpha_used;
////    uint render_fresnel;
////    float u_div;
////    float v_div;

////    float4 decal_uv_rect; // TODO: is this value for statics only?

////    float4 color_no_tex;
////    float4 color1;
////    float4 color2;
////    float4 color3;

////    uint PisTextureSRGB[64];

////}