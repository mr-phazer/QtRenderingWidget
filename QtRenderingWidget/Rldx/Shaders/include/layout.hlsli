// layout for vertex INPUT from app

struct VertexInputType
{   
    float4 position : POSITION;
    float2 tex1 : TEXCOORD0;
    float2 tex2 : TEXCOORD1;

    float3 normal : NORMAL;    
    float3 tangent : TANGENT;    
    float3 binormal : BINORMAL;    

    float4 color : COLOR1;
    
    uint4 BoneIndices : BLENDINDICES;
    float4 Weights : BLENDWEIGHTS;

    

    //int SV_InstanceID
};

// layout for vertex output to the pixel shader
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex1 : TEXCOORD0;
    float2 tex2 : TEXCOORD1;

    float3 normal : NORMAL;

    float3 tangent : TANGENT;

    float3 binormal : BINORMAL;

    float4 color : COLOR1;
    //float4 color_tangent : COLOR2;
    //float4 color_normal : COLOR3;
    float3 viewDirection : TEXCOORD2;
    float3 eyePos : TEXCOORD3;
    
    float4 Wpos : TEXCOORD6;
//    float3 UVW : TEXCOORD11;

    //float4x4 mViewI : TEXCOORD7;
    float4 screenPos : TEXCOORD5;

    //float2 shadow_tex : iTEXCOORD6;
    //float4 lightSpaceposition : POSITION2;
    //float3 lRay : POSITION3;
    //float3 view : POSITION4;

};

//struct SSAO_PixelInputType
//{
//    float4 positionV : SV_POSITION;
//    //float4 positionV : SV_POSITION;
//    float3 positionH : TEXCOORD0;

//    float2 tex1 : TEXCOORD1;
//    float2 tex2 : TEXCOOR2;

//    float4 normalV : NORMAL;

//    //float3 tangent : TANGENT;
//    //float3 binormal : BINORMAL;

//    //float4 color : COLOR;
//    //float3 viewDirection : TEXCOORD1;

//};

struct Quad_VertexInputType
{
    float4 position : POSITION;
    float2 tex1 : TEXCOORD;
    float4 color : COLOR1;
};

struct Quad_PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex1 : TEXCOORD0;
    float4 color : COLOR2;
    float3 cubemap_tex1 : TEXCOORD1;
    float3 wpos : TEXCOORD2;
    float3 viewDir : TEXCOORD3;
};

struct Quad_BLur_PixelInputType
{
    float4 position : SV_POSITION;

    float2 tex1 : TEXCOORD0;

    float2 texCoord1 : texCoordCOORD0;
    float2 texCoord2 : texCoordCOORD1;
    float2 texCoord3 : texCoordCOORD2;
    float2 texCoord4 : texCoordCOORD3;
    float2 texCoord5 : texCoordCOORD4;
    float2 texCoord6 : texCoordCOORD5;
    float2 texCoord7 : texCoordCOORD6;
    float2 texCoord8 : texCoordCOORD7;
    float2 texCoord9 : texCoordCOORD9;
    float2 texCoord10 : texCoordCOORD10;
    float2 texCoord11 : texCoordCOORD11;
    float2 texCoord12 : texCoordCOORD12;

    float2 texCoord13 : texCoordCOORD13;

    float texelSize : texCoordCOORD14;

    float4 color : COLOR3;

};

void make_debug_color(in VertexInputType input, inout PixelInputType output)
{

}