#pragma once

#include "..\..\ImportExport\FileFormats\RigidModel\Types\Common\RigidModelFile.h"
#include "..\..\ImportExport\GeneralTypes\Animation\Skeleton.h"
#include "..\DataTypes\DxMeshData.h"

namespace rldx
{
	class DxMeshCreatorHelper
	{
	public:
		static DxCommonMeshData MakeTestCubeMesh(ID3D11Device* poDevice);
		static DxCommonMeshData MakeGrid(ID3D11Device* poDevice, int linesPerAxis = 10, float spacing = 0.01f);
		static DxCommonMeshData CreateFromRmv2Mesh(ID3D11Device* poDevice, const rmv2::MeshBlockCommon& rmv2Mesh);
		static DxCommonMeshData CreateSkeletonMesh(ID3D11Device* poDevice, const skel_anim::Skeleton& animFileSkeleton);
	};
}
