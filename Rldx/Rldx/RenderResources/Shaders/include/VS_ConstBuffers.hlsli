struct LightStruct
{
    float3 position;
    uint reserved1;
    
    float3 direction;    
    float radiance;
};


cbuffer VS_PerScene_ConstantBuffer : register(b0)
{
    float3 eyePosition;
    uint reserved1;
                                                 
    float3 eyeDirection;
    uint radiance;
    
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProjection;

    //LightStruct light[1];
};


//cbuffer VS_PerMesh_ConstantBuffer : register(b0)
//{
//    float4x4 mPerMesh_World;
//    float4 color;

//    float4x4 rot_x;
//    float4x4 rot_y;
//    float4x4 rot_z;
//    float4x4 mPivot;

//    float3 pivot;
//    int _reserved;

//    uint show_reflections;
//    uint is_diffuse_gamma;
//    uint is_specular_gamma;
//    uint has_alpha;
    
//    LightStruct light;
    
//    float4x4 __tranforms[256]; // TODO: Delete ( move to skeleton buffer)
//    float4x4 __inverse[256]; // TODO: Delete ( move to skeleton buffer)    
//};

//cbuffer VS_PerFrame_ConstantBuffer : register(b1)
//{
//    float4x4 mView;
//    float4x4 mProjection;

//    float4x4 mLightView;
//    float4x4 mLightProjection;

//    float4x4 mViewInverse;
//    float4x4 mPerFrameWorld;

//    float3 eyePosition;
//    float pad1;

//    float3 lightPosition;
//    float pad2;

//    float4 screenRes;
//}

//cbuffer VVS_PerFrame_ConstantBuffer : register(b1)
//{
//    float4x4 mView;
//    float4x4 mProjection;

//    float4x4 mLightView;
//    float4x4 mLightProjection;

//    float4x4 mViewInverse;
//    float4x4 mPerFrameWorld;

//    float3 eyePosition;
//    float pad1;

//    float3 lightPosition;
//    float pad2;

//    float4 screenRes;
//}


//cbuffer VS_PerMesh_SkeletonConstantBuffer : register(b2)
//{                      
//    uint bone_count;
//    float reserved21;
//    float reserved22;
//    float reserved23;
    
//    float4x4 tranforms[256];
//    float4x4 inverse[256];
//};


////cbuffer PerIndiVidualModelConstantBuffer : register(b0)
////{
////    struct _VS_Mesh
////    {
////        float4x4 mPerMesh_World;

////    //float3 cameraPosition;
////    //int bone_index;

////    //float3 cameraLookAt;
////    //int static_model;

////        float4 color;

////        float4x4 rot_x;
////        float4x4 rot_y;
////        float4x4 rot_z;
////        float4x4 mPivot;

////        float3 pivot;
////        uint use_ao;

////        uint show_reflections;
////        uint is_diffuse_gamma;
////        uint is_specular_gamma;
////        uint has_alpha;

////        float4x4 tranforms[256];
////        float4x4 inverse[256];
        
////    } VS_Mesh;
////};
