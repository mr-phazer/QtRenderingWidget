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

	class DxSkeletonMeshCreator
	{
	public:
		DxCommonMeshData Create(ID3D11Device* poDevice, const skel_anim::Skeleton& animFileSkeleton)
		{
			std::vector<uint32_t> indices;
			std::vector<CommonVertex> vertices;
			FillLineIndices(animFileSkeleton, indices);
			FillLineVertices(animFileSkeleton, vertices);

			//TODO: remove to set mesh to "line list" (should it be called "mesh" then?, maybe rename to "CommonPrimitive"?)
		}

	private:
		void FillLineIndices(const skel_anim::Skeleton& skeleton, std::vector<uint32_t>& indices)
		{
			for (const auto& itBone : skeleton.boneTable)
			{
				// line from parent to child
				indices.push_back(itBone.parentIndex);
				indices.push_back(itBone.boneIndex);

			}
		}

		void FillLineVertices(const skel_anim::Skeleton& skeleton, std::vector<CommonVertex>& vertuces)
		{
			for (const auto& itBone : skeleton.boneTable)
			{


				/*
				ALGO:
					- finish/use FrameGenerator to generate bind pose matrices
					- for every parent-child bone pair, tranform {0,0,0} by bone global tranform matrix

					this ought to fint with line indices made above

				*/

			}
		}
	};
}
