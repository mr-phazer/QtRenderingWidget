#pragma once

#include "..\..\..\ImportExport\FileFormats\Anim\Types\Common\TwAnimFile.h"
#include "..\..\..\ImportExport\GeneralTypes\Animation\AnimationManager.h"
#include "..\..\ImportExport\FileFormats\RigidModel\Types\Common\RigidModelFile.h"
#include "..\..\ImportExport\GeneralTypes\Animation\Skeleton.h"

#include <vector>
#include "..\Creators\DxMeshCreator.h"
#include "..\DataTypes\DxMeshData.h"

namespace rldx
{
	class DxMeshCreatorHelper
	{
	public:
		static DxCommonMeshData MakeTestCubeMesh(ID3D11Device* poDevice);
		static DxCommonMeshData MakeGrid(ID3D11Device* poDevice, int linesPerAxis = 10, float spacing = 0.01f);
		static DxCommonMeshData CreateFromRmv2Mesh(ID3D11Device* poDevice, const rmv2::MeshBlockCommon& rmv2Mesh);
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



			auto meshCreator = DxMeshRenderDataCreator<CommonVertex, uint32_t>();
			auto result = meshCreator.CreateDxMeshRenderData(poDevice, vertices, indices);

			result.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

			return result;

			//TODO: remove to set mesh to "line list" (should it be called "mesh" then?, maybe rename to "CommonPrimitive"?)
		}

	private:
		void FillLineIndices(const skel_anim::Skeleton& skeleton, std::vector<uint32_t>& indices)
		{
			auto index = 0;
			for (const auto& itBone : skeleton.boneTable)
			{
				// line from parent to child
				indices.push_back(index++);
				indices.push_back(index++);
			}
		}

		void FillLineVertices(const skel_anim::Skeleton& skeleton, std::vector<CommonVertex>& vertices)
		{

			for (uint32_t boneIndex = 0; boneIndex < skeleton.boneTable.size(); boneIndex++)
			{
				auto childPosition = sm::Vector3::Transform({ 0,0,0 }, skeleton.bindposeMatrices[boneIndex]);
				sm::Vector3 parentPosition = { 0,0,0 };

				CommonVertex parentVertex;
				parentVertex.boneIndices = { 0, 0,0,0 };
				CommonVertex childVertex;

				if (skeleton.boneTable[boneIndex].parentIndex != -1)
				{
					parentPosition = sm::Vector3::Transform({ 0,0,0 }, skeleton.bindposeMatrices[skeleton.boneTable[boneIndex].parentIndex]);
					parentVertex.boneIndices = { static_cast<uint32_t>(skeleton.boneTable[boneIndex].parentIndex),0,0,0 };
				}

				childVertex.position = { childPosition.x, childPosition.y, childPosition.z, 0 };
				childVertex.weights = { 1,0,0,0 };
				childVertex.boneIndices = { boneIndex,0,0,0 };
				vertices.push_back(childVertex);

				parentVertex.position = { childPosition.x, childPosition.y, childPosition.z, 0 };
				parentVertex.weights = { 1,0,0,0 };
				vertices.push_back(parentVertex);

			};
		};
	};
}
