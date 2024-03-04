#pragma once

#include <SimpleMath.h>
#include <vector>

struct SkeletonBoneTransform
{
	sm::Vector3 position = { 0,0,0 };
	sm::Quaternion rotation = sm::Quaternion::Identity;
	sm::Vector3 scale = { 0,0,0 };;
};

struct SkeletonBoneNode
{
	SkeletonBoneNode* parent;
	SkeletonBoneTransform transform;
};

struct Skeleton
{
	std::vector< SkeletonBoneNode> bonesNodes;
	std::vector<sm::Matrix> inverseBindposeMatrices;
};

struct SkeletonFramme
{
	std::vector< SkeletonBoneNode> bonesNodes;
};