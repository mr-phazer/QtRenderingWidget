#pragma once


#include <vector>
#include <ImportExport\FileFormats\RigidModel\Types\Common\RigidModelFile.h>
#include "..\Creators\DxMeshCreator.h"
#include "..\DataTypes\DxMeshData.h"

#include <Rldx\Rldx\Animation\DataTypes\Skeleton.h>

namespace rldx
{
	// TODO: remove?
		//extern TRawMeshData<DirectX::XMFLOAT3, uint32_t> cubeMeshDat;

	class DxMeshCreatorHelper
	{
	public:
		static DxCommonMeshData MakeTestCubeMesh(ID3D11Device* poDevice);
		static DxCommonMeshData MakeGrid(ID3D11Device* poDevice, int linesPerAxis = 10, float spacing = 0.01f);
		static DxCommonMeshData CreateFromRmv2Mesh(ID3D11Device* poDevice, const rmv2::MeshBlockCommon& rmv2Mesh);
		static DxCommonMeshData MakeBoundingBoxMesh(ID3D11Device* poDevice, const DirectX::BoundingBox& bb, const sm::Color& color);
	};

	class IDxMeshCreator
	{
		virtual DxCommonMeshData CreateMesh(ID3D11Device* poDevice) = 0;
	};

	class Rmv2MeshCreator : IDxMeshCreator
	{
		const rmv2::MeshBlockCommon& m_rmv2Mesh;

	public:
		Rmv2MeshCreator(const rmv2::MeshBlockCommon& rmv2Mesh)
			:
			m_rmv2Mesh(rmv2Mesh)
		{}

		DxCommonMeshData CreateMesh(ID3D11Device* poDevice) override;
	};

	class DxSkeletonMeshCreator
	{
	public:
		static DxCommonMeshData Create(ID3D11Device* poDevice, const skel_anim::Skeleton& animFileSkeleton)
		{
			std::vector<uint32_t> indices;
			std::vector<CommonVertex> vertices;

			FillLineIndices(animFileSkeleton, indices);
			FillLineVertices(animFileSkeleton, vertices);

			FillCubeIndices(animFileSkeleton, indices);
			FillCubeVertices(animFileSkeleton, vertices);

			auto meshCreator = DxMeshRenderDataCreator<CommonVertex, uint32_t>();
			auto newMesh = meshCreator.CreateDxMeshRenderData(poDevice, vertices, indices);

			// # set to line-rendering
			newMesh.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

			return newMesh;
		}

	private:
		static void FillLineIndices(const skel_anim::Skeleton& skeleton, std::vector<uint32_t>& indices)
		{
			auto index = 0;
			for (const auto& itBone : skeleton.GetBoneTable())
			{
				// line from parent to child
				indices.push_back(index++);
				indices.push_back(index++);
			}
		}
		static void FillLineVertices(const skel_anim::Skeleton& skeleton, std::vector<CommonVertex>& vertices)
		{
			for (uint32_t boneIndex = 0; boneIndex < skeleton.GetBoneTable().size(); boneIndex++)
			{

				auto& m_bindposeMatrices = skeleton.GetBindPoseMatrices();

				// transformMatrix
				auto bonePosition = sm::Vector3::Transform({ 0,0,0 }, m_bindposeMatrices[boneIndex]);

				// TODO: REMOVE: testing if 4d transformMatrix give the same value
				// TODO: EDIT: It does ONLY, if w = 1.0
				auto test4d_bonePosition2 = sm::Vector4::Transform({ 0,0,0,1 }, m_bindposeMatrices[boneIndex]);

				sm::Vector3 parentBonePosition = { 0,0,0 };

				CommonVertex parentVertex;
				parentVertex.boneIndices = { 0, 0,0,0 };

				CommonVertex childVertex;

				auto& bindposeMatrices = skeleton.GetBindPoseMatrices();
				auto& boneTable = skeleton.GetBoneTable();

				if (boneTable[boneIndex].parentIndex != -1)
				{
					parentBonePosition = sm::Vector3::Transform({ 0,0,0 }, bindposeMatrices[boneTable[boneIndex].parentIndex]);
					parentVertex.boneIndices = { static_cast<uint32_t>(boneTable[boneIndex].parentIndex),0,0,0 };
				}

				childVertex.position = { bonePosition.x, bonePosition.y, bonePosition.z, 0 };
				childVertex.weights = { 1,0,0,0 };
				childVertex.boneIndices = { boneIndex,0,0,0 };
				childVertex.color = { 0.2f,0.2f,0.2f,1 };
				vertices.push_back(childVertex);

				parentVertex.position = { parentBonePosition.x, parentBonePosition.y, parentBonePosition.z, 0 };
				parentVertex.weights = { 1,0,0,0 };
				parentVertex.color = { 0.4f,0.4f,0.4f,1 };
				vertices.push_back(parentVertex);

			};
		};

		static void FillCubeVertices(const skel_anim::Skeleton& skeleton, std::vector<CommonVertex>& vertices)
		{
			float DEBUG_offset = 0.0f;

			for (uint32_t boneIndex = 0; boneIndex < skeleton.GetBoneTable().size(); boneIndex++)
			{
				AddCube(skeleton.GetBindPoseMatrices()[boneIndex], vertices, boneIndex);
			}
		}

		static void FillCubeIndices(const skel_anim::Skeleton& skeleton, std::vector<uint32_t>& indices)
		{
			uint32_t startOffset = static_cast<uint32_t>(indices.size());

			uint32_t vertexOffset = 0;

			for (uint32_t boneIndex = 0; boneIndex < skeleton.GetBoneTable().size(); boneIndex++)
			{
				for (auto& itCubeIndex : sm_cubeMeshData.indices)
				{
					indices.push_back(itCubeIndex + startOffset + vertexOffset);
				}

				vertexOffset += static_cast<uint32_t>(sm_cubeMeshData.vertices.size());
			}
		};

		static void AddCube(const sm::Matrix& transformMatrix, std::vector<CommonVertex>& vertices, uint32_t boneINdex)
		{
			const auto cubeScaleFactor = 1 / 70.0f;

			for (auto& itCubeVertex : sm_cubeMeshData.vertices)
			{
				CommonVertex newVertex;
				newVertex.color = { 0.5f,0.5f,1,1 };

				auto position =
					sm::Vector3::Transform(
						sm::Vector3(itCubeVertex.x, itCubeVertex.y, itCubeVertex.z) * cubeScaleFactor,
						transformMatrix);

				newVertex.position = sm::Vector4(position.x, position.y, position.z, 1);
				newVertex.weights = { 1,0,0,0 };
				newVertex.boneIndices = { boneINdex,0,0,0 };

				vertices.push_back(newVertex);
			}
		}

		static TRawMeshData<DirectX::XMFLOAT3, uint32_t> sm_cubeMeshData;
	};
}
