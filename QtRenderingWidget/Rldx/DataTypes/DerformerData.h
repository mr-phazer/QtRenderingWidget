#pragma once

#include <SimpleMath.h>

const size_t MAX_BONES = 256;

/// <summary>
/// Data for 1 skeleton, used for skinning
/// Simplest possiible implementation, static arrays of matrices
/// </summary>
struct DxDeformerData
{
	uint32_t boneCount;
	uint32_t reserved[3]; // padding for 16 byte alignment, maybe use this for something later
	
	sm::Matrix framePoseMatrices[MAX_BONES];
	sm::Matrix inverseBindPoseMatrices[MAX_BONES];
};