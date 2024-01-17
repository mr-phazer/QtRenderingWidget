static const int t_Diffuse = 0;
static const int t_Normal = 1;
static const int t_Mask = 3;
static const int t_AmbientOcclusion = 5;
static const int t_TilingDirtUV2 = 7;
static const int t_DirtAlphaMask = 8;
static const int t_SkinMask = 10;
static const int t_Specular = 11;
static const int t_GlossMap = 12;
static const int t_DecalDirtmap = 13;
static const int t_DecalDirtmask = 14;
static const int t_DecalMask = 15;
static const int t_DiffuseDamage = 17;
static const int t_BaseColor = 27;
static const int t_MaterialMap = 29;

static const int t_DecalDiffuse = t_Diffuse + 30;
static const int t_DecalNormal = t_Normal + 30;
static const int t_DecalSpecular = t_Specular + 30;

sampler SamplerLinear : register(s0);
sampler s_anisotropic : register(s1);
sampler s_point_clamp : register(s2);
sampler s_normal : register(s3);
sampler s_mask : register(s4);
sampler s_point : register(s5);
sampler s_AnisoClamp : register(s6);

TextureCube tex_cube_diffuse : register(t0);
TextureCube tex_cube_specular : register(t1);
Texture2D tex_brdf_lut : register(t2);
Texture2D ao_mapTexture : register(t44);
Texture2D Shadow_mapTexture : register(t45);
Texture2D Quad_Texture : register(t46);

/*
    Mesh textures,
    texture type id are index to this texture array

*/
Texture2D shaderTextures[64] : register(t47); 


//SamplerState ReflectSampler;
//SamplerState s_base_colour;
//SamplerState s_normal;
//SamplerState s_detail_normal;
//SamplerState s_roughness;
//SamplerState s_metalness;
//SamplerState s_mask1;
//SamplerState s_mask2;
//SamplerState s_mask3;
//SamplerState s_ambient_occlusion;
//SamplerState s_dirtmap_uv2;
//SamplerState s_alpha_mask;
//SamplerState s_decal_diffuse;
//SamplerState s_decal_normal;
//SamplerState s_decal_mask;
//SamplerState s_decal_dirtmap;
//SamplerState s_decal_dirtmask;
//SamplerState s_diffuse_damage;
//SamplerState s_anisotropy;
//SamplerState s_soft_normal;
//SamplerState s_curvature;
//SamplerState s_translucency;

//SamplerState s_cubemap
//{
//    Filter = ANISOTROPIC;
//    AddressU = CLAMP;
//    AddressV = CLAMP;
//    AddressW = CLAMP;
//};
