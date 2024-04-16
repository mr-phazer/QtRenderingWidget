#include "include/layout.hlsli"
#include "include/VS_ConstBuffers.hlsli"


void doSkinnning_New(
    in VertexInputType input,
	int weights_per_vertex,
	out float4 position,
	out float3 normal,
	out float3 tangent,
	out float3 bitangent)
{
    float4x4 skinningMatrix = 0;
		
    [unroll]
    for (int i = 0; i < weights_per_vertex; i++)
    {
        skinningMatrix += tranforms[input.BoneIndices[i]] * input.Weights[i];

        position.xyz = mul(position, skinningMatrix).xyz;
        normal = mul(normal, (float3x3) skinningMatrix);
        tangent = mul(normal, (float3x3) skinningMatrix);
        bitangent = mul(normal, (float3x3) skinningMatrix);
    }
}

static const int weights_per_vertex = 4;

void DoSkinning(
	in VertexInputType input,
	out float4 position,
	out float3 normal,
	out float3 tangent,
	out float3 binormal)
{
    // the matrices used for this vertex' transformation
    float4x4 FramePoseMatrices[4];
    FramePoseMatrices[0] = tranforms[input.BoneIndices.x];
    FramePoseMatrices[1] = tranforms[input.BoneIndices.y];
    FramePoseMatrices[2] = tranforms[input.BoneIndices.z];
    FramePoseMatrices[3] = tranforms[input.BoneIndices.w];

    position = 0;
    normal = 0;
    tangent = 0;
    binormal = 0;
	
	[unroll]
    for (int i = 0; i < weights_per_vertex; i++)
    {
		// transform vertex position
        position +=
			input.Weights[i] * mul(float4(input.position.xyz, 1), FramePoseMatrices[i]);

		// tranform lighting vectors, only use rotation part (3x3) of matrices 
        normal.xyz +=
			input.Weights[i] * mul(input.normal.xyz, (float3x3) FramePoseMatrices[i]);
        tangent.xyz +=
			input.Weights[i] * mul(input.tangent.xyz, (float3x3) FramePoseMatrices[i]);
        binormal.xyz +=
			input.Weights[i] * mul(input.binormal.xyz, (float3x3) FramePoseMatrices[i]);

    }
}


PixelInputType main(in VertexInputType input)
{
    PixelInputType output;

#if 1     
    DoSkinning(input,
	output.position,
	output.normal,
	output.tangent,
	output.binormal
	);
#else
	//#define DEBUG_NO_SKINING	
	
    output.position.xyz = input.position.xyz;
    output.normal.xyz = input.normal.xyz;
    output.tangent.xyz = input.tangent.xyz;
    output.binormal.xyz = input.binormal.xyz;
#endif	
		
    
	
    output.position = mul(float4(output.position.xyz + pivot, 1), weaponWorld);
	
    
    float4x4 mWorldMultipled = nodeWorld;    
    output.position = mul(float4(output.position.xyz, 1), mWorldMultipled);
    
	// -- save world space position later calculations	
    output.Wpos = output.position;
	

	// ---- tranform normal, tangent, bitagent  (only world for normal and tangents, as they are not drawn) -----
    output.normal.xyz = mul(output.normal.xyz, (float3x3) mWorldMultipled);
    output.normal.xyz = normalize(output.normal.xyz);

    output.tangent.xyz = mul(output.tangent.xyz, (float3x3) mWorldMultipled);
    output.tangent.xyz = normalize(output.tangent.xyz);

    output.binormal.xyz = mul(output.binormal.xyz, (float3x3) mWorldMultipled);
    output.binormal.xyz = normalize(output.binormal.xyz);
	
    output.position = mul(float4(output.position.xyz, 1), mView);
    output.position = mul(float4(output.position.xyz, 1), mProjection);

	// save screen position for screen-space stuff
    output.screenPos = output.position;
	
	// UVs
    output.tex1 = input.tex1;
    output.tex2 = input.tex2;
	
    
    output.viewDirection.xyz = normalize(
	eyePosition - output.Wpos.xyz);

    //output.eyePos = (float3) mViewInverse[3];
    output.eyePos = eyePosition;
    
    output.viewDirection.xyz = normalize(
	eyePosition - output.Wpos.xyz);

    
// TODO : remove?                                  
    //output.eyePos = mViewInverse[3];
    //output.Wpos = float4(output.Wpos.xyz, 1);;
    //output.mViewI = mViewInverse;

	// vertex color
    output.color = input.color;

    return output;
}

//#include "layout.hlsli"
//#include "VS_ConstBuffers.hlsli"

//PixelInputType trans_to_lightView(VertexInputType input)
//{
//    PixelInputType output;
//    float4 position = float4(input.position.xyz, 1.0f);

//    // Transform the vertex positionition into projected space.
//    float4 modelposition = mul(position, mPerFrameWorld);
//    position = mul(modelposition, mView);
//    position = mul(position, mProjection);
//    output.position = position;

//    // Transform the vertex positionition into projected space from the POV of the light.
//    float4 lightSpaceposition = mul(modelposition, mLightView);
//    lightSpaceposition = mul(lightSpaceposition, mLightProjection);
//    output.lightSpaceposition = lightSpaceposition;

//    // Light ray
//    float3 lRay = lightPosition.xyz - modelposition.xyz;
//    output.lRay = lRay;

//    // Camera ray
//    output.view = eyePosition.xyz - modelposition.xyz;

//    // Transform the vertex normal into world space.
//    //float4 norm = float4(input.normal, 1.0f);
//    //norm = mul(norm, model);
//    //output.norm = norm.xyz;

//    // Pass through the color and texture coordinates without modification.
//    //output.color = input.color;
//    //output.tex = input.tex;

//    return output;
//}

//PixelInputType main(in VertexInputType input)
//{
//    PixelInputType output = trans_to_lightView(input);

//    float4x4 FramePoseMatrices[4];
//    FramePoseMatrices[0] = mul(inverse[input.BoneIndices.x], tranforms[input.BoneIndices.x]);
//    FramePoseMatrices[1] = mul(inverse[input.BoneIndices.y], tranforms[input.BoneIndices.y]);
//    FramePoseMatrices[2] = mul(inverse[input.BoneIndices.z], tranforms[input.BoneIndices.z]);
//    FramePoseMatrices[3] = mul(inverse[input.BoneIndices.w], tranforms[input.BoneIndices.w]);

//    output.position = 0;
//    output.normal = 0;
//    output.tangent = 0;
//    output.binormal = 0;

//    [unroll]
//    for (int i = 0; i < 4; i++)
//    {
//        output.position.xyz += input.Weights[i] * mul(float4(input.position.xyz, 1), FramePoseMatrices[i]);
//        output.normal.xyz += input.Weights[i] * mul(input.normal.xyz, (float3x3) FramePoseMatrices[i]); // only use rotation part (3x3) of matrices
//        output.tangent.xyz += input.Weights[i] * mul(input.tangent.xyz, (float3x3) FramePoseMatrices[i]);
//        output.binormal.xyz += input.Weights[i] * mul(input.binormal.xyz, (float3x3) FramePoseMatrices[i]);
//    }

//    ////TODO: DEBUG: these lines disable animations
//    //output.position.xyz = input.position.xyz;
//    //output.normal.xyz = input.normal.xyz;
//    //output.tangent.xyz = input.tangent.xyz;
//    //output.binormal.xyz = input.binormal.xyz;
//    //// ---

//    //  world transform
//    float4x4 mPerFrameWorld = mul(mPerMesh_World, mPerFrameWorld);
//    output.position = mul(output.position, mPerFrameWorld);

//    // save world space position for camera calculations
//    float4 worldPosition = output.position;

//    output.position = mul(float4(output.position.xyz, 1), mView);
//    output.position = mul(float4(output.position.xyz, 1), mProjection);

//    output.screenPos = output.position;
//    //output.screenPos.y *= -1;

//    // ---- tranform normal, tangent, bitagent  (only world for normal and tangents, as they are not drawn) -----
//    output.normal.xyz = mul(output.normal.xyz, (float3x3) mPerFrameWorld);
//    output.normal.xyz = normalize(output.normal.xyz);

//    output.tangent.xyz = mul(output.tangent.xyz, (float3x3) mPerFrameWorld);
//    output.tangent.xyz = normalize(output.tangent.xyz);

//    output.binormal.xyz = mul(output.binormal.xyz, (float3x3) mPerFrameWorld);
//    output.binormal.xyz = normalize(output.binormal.xyz);

//    //output.normal.yxz = mul(output.normal.yxz, (float3x3) View);
//    //output.normal.yxz = normalize(output.normal.yxz);

//    //output.tangent.yxz = mul(output.tangent.yxz, (float3x3) View);
//    //output.tangent.yxz = normalize(output.tangent.yxz);

//    //output.binormal.yxz = mul(output.binormal.yxz, (float3x3) View);
//    //output.binormal.yxz = normalize(output.binormal.yxz);

//    // UVs
//    output.tex1 = input.tex1;

//    // calculate camera view direction for pixel shader
// //   output.viewDirection.xyz = normalize(
//	//(float3) mViewInverse[3] - worldPosition);

//    output.viewDirection.xyz = normalize(
//	eyePosition - worldPosition);

//    //output.eyePos = (float3) mViewInverse[3];
//    output.eyePos = eyePosition;
//    output.Wpos = worldPosition;

//    // vertex color (never used)
//    output.color = input.color;

//    return output;
//}

////#include "layout.hlsli"
////#include "VS_ConstBuffers.hlsli"

////PixelInputType trans_to_lightView(VertexInputType input)
////{
////    PixelInputType output = (PixelInputType) 0;
////    float4 position = float4(input.position.xyz, 1.0f);

////    // Transform the vertex positionition into projected space.
////    float4 modelposition = mul(position, mPerFrameWorld);
////    output.Wpos = modelposition;

////    position = mul(modelposition, mView);
////    position = mul(position, mProjection);
////    output.position = position;

////    // Transform the vertex positionition into projected space from the POV of the light.
////    float4 lightSpaceposition = mul(modelposition, mLightView);
////    lightSpaceposition = mul(lightSpaceposition, mLightProjection);
////    output.lightSpaceposition = lightSpaceposition;

////    // Light ray
////    float3 lRay = lightPosition.xyz - modelposition.xyz;
////    output.lRay = lRay;

////    // Camera ray
////    output.view = eyePosition.xyz - modelposition.xyz;

////    // Transform the vertex normal into world space.
////    //float4 norm = float4(input.normal, 1.0f);
////    //norm = mul(norm, model);
////    //output.norm = norm.xyz;

////    // Pass through the color and texture coordinates without modification.
////    //output.color = input.color;
////    //output.tex = input.tex;

////    return output;
////}

////PixelInputType main(in VertexInputType input)
////{
////    PixelInputType output = trans_to_lightView(input);

////    float4x4 mPerFrameWorld = mPerFrameWorld;

////    output.screenPos = output.Wpos;
////    //output.screenPos.y *= -1;
////    output.normal.xyz = input.normal.xyz;
////    output.tangent.xyz = input.tangent.xyz;;
////    output.binormal.xyz = input.binormal.xyz;;
////    // ---- tranform normal, tangent, bitagent  (only world for normal and tangents, as they are not drawn) -----
////    output.normal.xyz = mul(output.normal.xyz, (float3x3) mPerFrameWorld);
////    output.normal.xyz = normalize(output.normal.xyz);

////    output.tangent.xyz = mul(output.tangent.xyz, (float3x3) mPerFrameWorld);
////    output.tangent.xyz = normalize(output.tangent.xyz);

////    output.binormal.xyz = mul(output.binormal.xyz, (float3x3) mPerFrameWorld);
////    output.binormal.xyz = normalize(output.binormal.xyz);

////    // UVs
////    output.tex1 = input.tex1;

////    output.viewDirection.xyz = normalize(
////	eyePosition - output.Wpos.xyz);

////    //output.eyePos = (float3) mViewInverse[3];
////    output.eyePos = eyePosition;
////    //output.Wpos = output.Wpos;

////    // vertex color (never used)
////    output.color = input.color;

////    return output;

////    return output;

////    float4x4 FramePoseMatrices[4];
////    FramePoseMatrices[0] = mul(inverse[input.BoneIndices.x], tranforms[input.BoneIndices.x]);
////    FramePoseMatrices[1] = mul(inverse[input.BoneIndices.y], tranforms[input.BoneIndices.y]);
////    FramePoseMatrices[2] = mul(inverse[input.BoneIndices.z], tranforms[input.BoneIndices.z]);
////    FramePoseMatrices[3] = mul(inverse[input.BoneIndices.w], tranforms[input.BoneIndices.w]);

////    output.position = 0;
////    output.normal = 0;
////    output.tangent = 0;
////    output.binormal = 0;

////    [unroll]
////    for (int i = 0; i < 4; i++)
////    {
////        output.position.xyz += input.Weights[i] * mul(float4(input.position.xyz, 1), FramePoseMatrices[i]);
////        output.normal.xyz += input.Weights[i] * mul(input.normal.xyz, (float3x3) FramePoseMatrices[i]); // only use rotation part (3x3) of matrices
////        output.tangent.xyz += input.Weights[i] * mul(input.tangent.xyz, (float3x3) FramePoseMatrices[i]);
////        output.binormal.xyz += input.Weights[i] * mul(input.binormal.xyz, (float3x3) FramePoseMatrices[i]);
////    }

////    ////TODO: DEBUG: these lines disable animations
////    //output.position.xyz = input.position.xyz;
////    //output.normal.xyz = input.normal.xyz;
////    //output.tangent.xyz = input.tangent.xyz;
////    //output.binormal.xyz = input.binormal.xyz;
////    //// ---

////    // -----------------------------

////    float4 position = float4(input.position.xyz, 1.0f);

////    // Transform the vertex positionition into projected space.
////        //float4 modelposition = mul(position, mPerFrameWorld);
////    float4 modelposition = position;
////    position = mul(modelposition, mView);
////    position = mul(position, mProjection);
////    output.position = position;

////    // Transform the vertex positionition into projected space from the POV of the light.
////    float4 lightSpaceposition = mul(modelposition, mLightView);
////    lightSpaceposition = mul(lightSpaceposition, mLightProjection);
////    output.lightSpaceposition = lightSpaceposition;

////    // Light ray
////    float3 lRay = lightPosition.xyz - modelposition.xyz;
////    output.lRay = lRay;

////    // Camera ray
////    output.view = eyePosition.xyz - modelposition.xyz;

////    // -----------------------------

////    //  world transform
////                //float4x4 mPerFrameWorld = mul(mPerMesh_World, mPerFrameWorld);
////                //output.position = mul(output.position, mPerFrameWorld);

////                //// save world space position for camera calculations
////                //float3 worldPosition = output.position.xyz;

////                //output.position = mul(float4(output.position.xyz, 1), mView);
////                //output.position = mul(float4(output.position.xyz, 1), mProjection);

////    output.screenPos = modelposition;
////    //output.screenPos.y *= -1;

////    // ---- tranform normal, tangent, bitagent  (only world for normal and tangents, as they are not drawn) -----
////    output.normal.xyz = mul(output.normal.xyz, (float3x3) mPerFrameWorld);
////    output.normal.xyz = normalize(output.normal.xyz);

////    output.tangent.xyz = mul(output.tangent.xyz, (float3x3) mPerFrameWorld);
////    output.tangent.xyz = normalize(output.tangent.xyz);

////    output.binormal.xyz = mul(output.binormal.xyz, (float3x3) mPerFrameWorld);
////    output.binormal.xyz = normalize(output.binormal.xyz);

////    // UVs
////    output.tex1 = input.tex1;

////    output.viewDirection.xyz = normalize(
////	eyePosition - modelposition.xyz);

////    //output.eyePos = (float3) mViewInverse[3];
////    output.eyePos = eyePosition;
////    output.Wpos = modelposition;

////    // vertex color (never used)
////    output.color = input.color;

////    return output;
////}

//////#include "layout.hlsli"
//////#include "VS_ConstBuffers.hlsli"

//////#if 1
//////void trans_to_lightView(in VertexInputType input,

//////    in float4 position,
//////    out float4 lightSpaceposition,
//////    out float3 Wpos,
//////    out float3 lRay,
//////    out float3 camView,
//////    out float4 screenPos
//////)
//////{
//////    //PixelInputType output;
//////        //float4 position = float4(input.position.xyz, 1.0f);
//////        //position = float4(input.position.xyz, 1.0f);

//////    // Transform the vertex positionition into projected space.
//////    //float4 modelposition = float4(position.xyz, 1);
//////    float4 modelposition = mul(float4(position.xyz, 1), mPerFrameWorld);
//////    Wpos = modelposition;

//////    //position = mul(modelposition, mView);
//////    //position = mul(position, mProjection);
//////    ////output.position =position;
//////    screenPos = position;

//////    // Transform the vertex positionition into projected space from the POV of the light.
//////    //float4 lightSpaceposition = mul(modelposition, mLightView);
//////    lightSpaceposition = mul(modelposition, mLightView);
//////    lightSpaceposition = mul(lightSpaceposition, mLightProjection);
//////        //output.lightSpaceposition = lightSpaceposition;

//////    // Light ray
//////    //float3 lRay = lightPosition.xyz - modelposition.xyz;
//////    lRay = lightPosition.xyz - modelposition.xyz;
//////    //output.lRay =lRay;

//////    // Camera ray
//////    camView = eyePosition.xyz - modelposition.xyz;

//////    // Transform the vertex normal into world space.
//////    //float4 norm = float4(input.normal, 1.0f);
//////    //norm = mul(norm, model);
//////    //output.norm = norm.xyz;

//////    // Pass through the color and texture coordinates without modification.
//////    //output.color = input.color;
//////    //output.tex = input.tex;

//////    //return output;
//////}

//////#endif

//////// applies weighted to the inputted vertex,
//////void applyWeightsToVertex(
//////    in VertexInputType inoutVert,
//////    out float4 position,
//////    out float4 normal,
//////    out float4 tangent,
//////    out float4 binormal
//////)
//////{

//////    float4x4 FramePoseMatrices[4];
//////    FramePoseMatrices[0] = mul(inverse[inoutVert.BoneIndices.x], tranforms[inoutVert.BoneIndices.x]);
//////    FramePoseMatrices[1] = mul(inverse[inoutVert.BoneIndices.y], tranforms[inoutVert.BoneIndices.y]);
//////    FramePoseMatrices[2] = mul(inverse[inoutVert.BoneIndices.z], tranforms[inoutVert.BoneIndices.z]);
//////    FramePoseMatrices[3] = mul(inverse[inoutVert.BoneIndices.w], tranforms[inoutVert.BoneIndices.w]);

//////    float4 position1 = 0;
//////    normal = 0;
//////    tangent = 0;
//////    binormal = 0;

//////    [unroll]
//////    for (int i = 0; i < 4; i++)
//////    {
//////        position1.xyz += inoutVert.Weights[i] * mul(float4(inoutVert.position.xyz, 1), FramePoseMatrices[i]);

//////        // for normal+tangents only use rotation part (3x3) of matrices
//////        normal.xyz += inoutVert.Weights[i] * mul(inoutVert.normal.xyz, (float3x3) FramePoseMatrices[i]);
//////        tangent.xyz += inoutVert.Weights[i] * mul(inoutVert.tangent.xyz, (float3x3) FramePoseMatrices[i]);
//////        binormal.xyz += inoutVert.Weights[i] * mul(inoutVert.binormal.xyz, (float3x3) FramePoseMatrices[i]);
//////    }

//////    position = float4(position1.xyz, 1);

//////}

//////PixelInputType main(VertexInputType input)
//////{
//////    PixelInputType output;

//////    float4x4 mPerFrameWorld = mPerFrameWorld;
//////    float4 positionWeighted; // = float4(input.position.xyz, 1.0f);

//////    applyWeightsToVertex(
//////                         input,
//////                         positionWeighted,
//////                         output.normal,
//////                         output.tangent,
//////                         output.binormal
//////                     );

//////    //output.normal = input.normal;
//////    //output.tangent = input.tangent;
//////    //output.binormal = input.binormal;

//////    // Transform the vertex positionition into projected space.

//////            //float4 modelposition = mul(position, mPerFrameWorld);
//////    float4 modelposition = positionWeighted;;
//////    output.Wpos = modelposition;

//////    //position = mul(modelposition, mView);

//////    float4 position = mul(positionWeighted, mView);
//////    position = mul(position, mProjection);
//////    output.position = position;
//////    output.screenPos = position;

//////    // Transform the vertex positionition into projected space from the POV of the light.
//////    float4 lightSpaceposition = mul(modelposition, mLightView);
//////    lightSpaceposition = mul(lightSpaceposition, mLightProjection);
//////    output.lightSpaceposition = lightSpaceposition;

//////    // Light ray
//////    float3 lRay = lightPosition.xyz - modelposition.xyz;
//////    output.lRay = lRay;

//////    // Camera ray
//////    output.view = eyePosition.xyz - modelposition.xyz;

//////    // Transform the vertex normal into world space.
//////    //float4 norm = float4(input.norm, 1.0f);
//////    //norm = mul(norm, model);
//////    //output.norm = norm.xyz;

//////    //// Pass through the color and texture coordinates without modification.
//////    //output.color = input.color;
//////    //output.tex = input.tex;

//////    // Pass through the color and texture coordinates without modification.
//////    //output.color = input.color;
//////    //output.tex = input.tex;

//////    // ---- tranform normal, tangent, bitagent  (only world for normal and tangents, as they are not drawn) -----
//////    output.normal.xyz = mul(output.normal.xyz, (float3x3) mPerFrameWorld);
//////    output.normal.xyz = normalize(output.normal.xyz);

//////    output.tangent.xyz = mul(output.tangent.xyz, (float3x3) mPerFrameWorld);
//////    output.tangent.xyz = normalize(output.tangent.xyz);

//////    output.binormal.xyz = mul(output.binormal.xyz, (float3x3) mPerFrameWorld);
//////    output.binormal.xyz = normalize(output.binormal.xyz);

//////    //output.normal.yxz = mul(output.normal.yxz, (float3x3) View);
//////    //output.normal.yxz = normalize(output.normal.yxz);

//////    //output.tangent.yxz = mul(output.tangent.yxz, (float3x3) View);
//////    //output.tangent.yxz = normalize(output.tangent.yxz);

//////    //output.binormal.yxz = mul(output.binormal.yxz, (float3x3) View);
//////    //output.binormal.yxz = normalize(output.binormal.yxz);

//////    // UVs
//////    output.tex1 = input.tex1;
//////    output.tex2 = input.tex2;

//////    // calculate camera view direction for pixel shader
////// //   output.viewDirection.xyz = normalize(
//////	//(float3) mViewInverse[3] - worldPosition);

//////    output.viewDirection.xyz = normalize(
//////	eyePosition - output.Wpos);

//////    //output.eyePos = (float3) mViewInverse[3];
//////    output.eyePos = eyePosition;
//////        //output.Wpos = worldPosition;

//////    // vertex color (never used)
//////    output.color = input.color;

//////    return output;

//////}

////////PixelInputType main(in VertexInputType input)
////////{
////////    PixelInputType output;
////////    output.lightSpaceposition = float4(0, 0, 0, 0);
////////    output.eyePos = float3(0, 0, 0);
////////    output.lRay = float3(0, 0, 0);
////////    output.tex1 = float3(0, 0, 0);
////////    output.lRay = float3(0, 0, 0);

////////    applyWeightsToVertex(
////////        input,
////////        output.position,
////////        output.normal,
////////        output.tangent,
////////        output.binormal
////////    );

////////    trans_to_lightView(input,
////////        output.position,
////////        output.lightSpaceposition,
////////        output.Wpos,
////////        output.lRay,
////////        output.view,
////////        output.screenPos
////////    );

////////    output.position = mul(float4(output.position.xyz, 1), mView);
////////    output.position = mul(output.position, mProjection);

////////#if 0
////////    float4x4 FramePoseMatrices[4];
////////    FramePoseMatrices[0] = mul(inverse[input.BoneIndices.x], tranforms[input.BoneIndices.x]);
////////    FramePoseMatrices[1] = mul(inverse[input.BoneIndices.y], tranforms[input.BoneIndices.y]);
////////    FramePoseMatrices[2] = mul(inverse[input.BoneIndices.z], tranforms[input.BoneIndices.z]);
////////    FramePoseMatrices[3] = mul(inverse[input.BoneIndices.w], tranforms[input.BoneIndices.w]);

////////    output.position = 0;
////////    output.normal = 0;
////////    output.tangent = 0;
////////    output.binormal = 0;

////////    [unroll]
////////    for (int i = 0; i < 4; i++)
////////    {
////////        output.position.xyz += input.Weights[i] * mul(float4(input.position.xyz, 1), FramePoseMatrices[i]);
////////        output.normal.xyz += input.Weights[i] * mul(input.normal.xyz, (float3x3) FramePoseMatrices[i]); // only use rotation part (3x3) of matrices
////////        output.tangent.xyz += input.Weights[i] * mul(input.tangent.xyz, (float3x3) FramePoseMatrices[i]);
////////        output.binormal.xyz += input.Weights[i] * mul(input.binormal.xyz, (float3x3) FramePoseMatrices[i]);
////////    }
////////#endif

////////    ////TODO: DEBUG: these lines disable animations
////////    //output.position.xyz = input.position.xyz;
////////    //output.normal.xyz = input.normal.xyz;
////////    //output.tangent.xyz = input.tangent.xyz;
////////    //output.binormal.xyz = input.binormal.xyz;
////////    //// ---

////////    //  world transform
////////    float4x4 mPerFrameWorld = mul(mPerMesh_World, mPerFrameWorld);
////////    //output.position = mul(output.position, mPerFrameWorld);
////////    //float4 modelposition = output.position;
////////    //// save world space position for camera calculations
////////    //float3 worldPosition = output.position.xyz;

////////    //output.position = mul(float4(output.position.xyz, 1), mView);
////////    //output.position = mul(float4(output.position.xyz, 1), mProjection);

////////    //output.screenPos = output.position;
////////    //output.screenPos.y *= -1;

////////            //// Transform the vertex positionition into projected space from the POV of the light.
////////            //float4 lightSpaceposition = mul(modelposition, mLightView);
////////            //lightSpaceposition = mul(lightSpaceposition, mLightProjection);
////////            //output.lightSpaceposition = lightSpaceposition;

////////            //// Light ray
////////            //float3 lRay = lightPosition.xyz - worldPosition.xyz;
////////            //output.lRay = lRay;

////////            //// Camera ray
////////            //output.view = eyePosition.xyz - worldPosition.xyz;

////////    // Transform the vertex normal into world space.
////////    //float4 norm = float4(input.normal, 1.0f);
////////    //norm = mul(norm, model);
////////    //output.norm = norm.xyz;

////////    // Pass through the color and texture coordinates without modification.
////////    //output.color = input.color;
////////    //output.tex = input.tex;

////////    // ---- tranform normal, tangent, bitagent  (only world for normal and tangents, as they are not drawn) -----
////////    output.normal.xyz = mul(output.normal.xyz, (float3x3) mPerFrameWorld);
////////    output.normal.xyz = normalize(output.normal.xyz);

////////    output.tangent.xyz = mul(output.tangent.xyz, (float3x3) mPerFrameWorld);
////////    output.tangent.xyz = normalize(output.tangent.xyz);

////////    output.binormal.xyz = mul(output.binormal.xyz, (float3x3) mPerFrameWorld);
////////    output.binormal.xyz = normalize(output.binormal.xyz);

////////    //output.normal.yxz = mul(output.normal.yxz, (float3x3) View);
////////    //output.normal.yxz = normalize(output.normal.yxz);

////////    //output.tangent.yxz = mul(output.tangent.yxz, (float3x3) View);
////////    //output.tangent.yxz = normalize(output.tangent.yxz);

////////    //output.binormal.yxz = mul(output.binormal.yxz, (float3x3) View);
////////    //output.binormal.yxz = normalize(output.binormal.yxz);

////////    // UVs
////////    output.tex1 = input.tex1;

////////    // calculate camera view direction for pixel shader
//////// //   output.viewDirection.xyz = normalize(
////////	//(float3) mViewInverse[3] - worldPosition);

////////    output.viewDirection.xyz = normalize(
////////	eyePosition - output.Wpos);

////////    //output.eyePos = (float3) mViewInverse[3];
////////    output.eyePos = eyePosition;
////////        //output.Wpos = worldPosition;

////////    // vertex color (never used)
////////    output.color = input.color;

////////    return output;
////////}
