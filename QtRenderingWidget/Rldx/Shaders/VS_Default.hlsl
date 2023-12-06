#include "include/layout.hlsli"
#include "include/VS_ConstBuffers.hlsli"

//PixelInputType trans_to_lightView(VertexInputType input)
//{
//    PixelInputType output;
//    float4 position = float4(input.position.xyz, 1.0f);
      
//    output.Wpos = position;
//    // Transform the vertex positionition into projected space.

//    position = mul(float4(position.xyz, 1), mPerMesh_World);

//    position = mul(float4(position.xyz, 1), mView);
//    position = mul(float4(position.xyz, 1), mProjection);
//    output.position = position;

//    // Transform the vertex positionition into projected space from the POV of the light.
//    float4 lightSpaceposition = mul(position, mLightView);
//    lightSpaceposition = mul(lightSpaceposition, mLightProjection);
//    output.lightSpaceposition = lightSpaceposition;

//    // Light ray
//    float3 lRay = lightPosition.xyz - position.xyz;
//    output.lRay = lRay;

//    // Camera ray
//    output.view = eyePosition.xyz - position.xyz;

//    // Transform the vertex normal into world space.
//    //float4 norm = float4(input.normal, 1.0f);
//    //norm = mul(norm, model);
//    //output.norm = norm.xyz;

//    // Pass through the color and texture coordinates without modification.
//    output.color = input.color;
//    //output.tesssssssssssssssssssx = input.tex;
//    output.color.rgb = float3(1, 0, 0);
//    //input.tangent;
//    return output;
//}

PixelInputType main(in VertexInputType input)
{
    PixelInputType output;
                                                                                             // "combine" perFrame and perModel world
    //float4x4 mWorld = mPerMesh_World /* * mPerFrameWorld*/;
    float4x4 mWorldMultipled = mPerMesh_World;
    //float4x4 mWorld = mPerMesh_World;

    output.position = mul(float4(input.position.xyz + pivot.xyz, 1), mWorldMultipled);

    // save world space position for camera calculations
    output.Wpos.xyz = output.position.xyz;
    output.Wpos.w = 0;

    output.position = mul(float4(output.position.xyz, 1), mView);
    output.position = mul(float4(output.position.xyz, 1), mProjection);

    output.screenPos = output.position;

    //output.screenPos = output.Wpos;
    //output.screenPos.y *= -1;

    //output.position.xyz = input.position.xyz;
    output.normal.xyz = input.normal.xyz;
    output.tangent.xyz = input.tangent.xyz;;
    output.binormal.xyz = input.binormal.xyz;;
    // ---- tranform normal, tangent, bitagent  (only world for normal and tangents, as they are not drawn) -----
    output.normal.xyz = mul(output.normal.xyz, (float3x3) mWorldMultipled);
    output.normal.xyz = normalize(output.normal.xyz);

    output.tangent.xyz = mul(output.tangent.xyz, (float3x3) mWorldMultipled);
    output.tangent.xyz = normalize(output.tangent.xyz);

    output.binormal.xyz = mul(output.binormal.xyz, (float3x3) mWorldMultipled);
    output.binormal.xyz = normalize(output.binormal.xyz);

    
    //output.position.xyz += output.normal.xyz * pivot.z;
    
    // UVs
    output.tex1 = input.tex1;
    output.tex2 = input.tex2;

    output.viewDirection.xyz = normalize(
	eyePosition - output.Wpos.xyz);

    //output.eyePos = (float3) mViewInverse[3];
    output.eyePos = eyePosition;
    

    // un-transformed normals/tangents (adjuated to be in the 0.. 1 interval)
    //output.color_tangent.rgb = (input.tangent.rgb + 1.f) / 2.f;
    //output.color_normal.rgb = (input.normal.rgb + 1.f) / 2.f;

    // vertex color
    output.color = input.color;
    //output.color_tangent.rgb = input.tangent;
    
    return output;

 //   //float4x4 PM = tranforms[bone_index];

 //   //float4x4 PM = mul(Tranforms[bone_index], inverse[bone_index])
 //   float4x4 PM = mul(inverse[bone_index], tranforms[bone_index]);
 //   //PM[1] = mul(inverse[input.BoneIndices.y], tranforms[input.BoneIndices.y]);
 //   //PM[2] = mul(inverse[input.BoneIndices.z], tranforms[input.BoneIndices.z]);
 //   //PM[3] = mul(inverse[input.BoneIndices.w], tranforms[input.BoneIndices.w]);

 //   output.position = 0;
 //   //output.position =
 //   //output.position = input.position;

 //   //output.position.xyz += pivot;

 //   output.normal = 0;
 //   output.tangent = 0;
 //   output.binormal = 0;

 //   ////[unroll]
 //   // //for (int i = 0; i < 1; i++)

 //   //output.position.xyz += /* inp
 //   //ut.Weights[i] * */mul(float4(input.position.xyz, 1), PM);
 //   //output.normal.xyz += /*input.Weights[i] * */mul(input.normal.xyz, (float3x3) PM); // only use rotation part (3x3) of matrices
 //   //output.tangent.xyz += /*input.Weights[i] * */mul(input.tangent.xyz, (float3x3) PM);
 //   //output.binormal.xyz += /*input.Weights[i] * */mul(input.binormal.xyz, (float3x3) PM);

 //   //DO: DEBUG: these lines disable animations
 //   output.position.xyz = input.position.xyz;
 //   output.normal.xyz = input.normal.xyz;
 //   output.tangent.xyz = input.tangent.xyz;;
 //   output.binormal.xyz = input.binormal.xyz;;

 //   //  world transform
 //   //float4x4 m = mul(PM, mWorld);
 //   //float4x4 m = PM;
 //   //float4x4 mWorld = mul(mPerMesh_World, mPerFrameWorld) * 10;
 //   float4x4 m = mWorld;
 //   output.position = mul(output.position, m);

 //   // save world space position for camera calculations
 //   float3 worldPosition = output.position.xyz;

 //   output.position = mul(float4(output.position.xyz, 1), mView);
 //   output.position = mul(float4(output.position.xyz, 1), mProjection);

 //   output.screenPos = output.position;
 //   output.screenPos.y *= -1;

 //   // ---- tranform normal, tangent, bitagent  (only world for normal and tangents, as they are not drawn) -----
 //   output.normal.xyz = mul(output.normal.xyz, (float3x3) m);
 //   output.normal.xyz = normalize(output.normal.xyz);

 //   output.tangent.xyz = mul(output.tangent.xyz, (float3x3) m);
 //   output.tangent.xyz = normalize(output.tangent.xyz);

 //   output.binormal.xyz = mul(output.binormal.xyz, (float3x3) m);
 //   output.binormal.xyz = normalize(output.binormal.xyz);

 //   //output.normal.yxz = mul(output.normal.yxz, (float3x3) View);
 //   //output.normal.yxz = normalize(output.normal.yxz);

 //   //output.tangent.yxz = mul(output.tangent.yxz, (float3x3) View);
 //   //output.tangent.yxz = normalize(output.tangent.yxz);

 //   //output.binormal.yxz = mul(output.binormal.yxz, (float3x3) View);
 //   //output.binormal.yxz = normalize(output.binormal.yxz);

 //   // UVs
 //   output.tex1 = input.tex1;

 //   // calculate camera view direction for pixel shader
 ////   output.viewDirection.xyz = normalize(
	////(float3) mViewInverse[3] - worldPosition);

 //   output.viewDirection.xyz = normalize(
	//eyePosition - worldPosition);

 //   //output.eyePos = (float3) mViewInverse[3];
 //   output.eyePos = eyePosition;
 //   //output.Wpos = worldPosition;

 //   // vertex color (never used)
 //   output.color = input.color;

 //   return output;
}