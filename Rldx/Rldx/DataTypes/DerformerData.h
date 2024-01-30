#pragma once

#include <memory>
#include <SimpleMath.h>

const size_t MAX_BONES = 256;

/// <summary>
/// data for 1 skeleton, used for skinning
/// Simplest possiible implementation, static arrays of matrices
/// </summary>
struct DxDeformerData
{
	using UniquePointer = std::unique_ptr<DxDeformerData>;

	uint32_t boneCount;
	uint32_t reserved1; // padding for 16 byte alignment, maybe use this for something later
	uint32_t reserved2; // padding for 16 byte alignment, maybe use this for something later
	uint32_t reserved3; // padding for 16 byte alignment, maybe use this for something later
	
	sm::Matrix framePoseMatrices[MAX_BONES];
	sm::Matrix inverseBindPoseMatrices[MAX_BONES];
};