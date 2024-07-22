#include "..\..\..\DirectXTK\Inc\GeometricPrimitive.h"
#include "..\Creators\DxMeshCreator.h"
#include "DxMeshCreatorHelper.h"

namespace rldx
{
	DxCommonMeshData DxMeshCreatorHelper::MakeTestCubeMesh(ID3D11Device* poDevice)
	{
		DirectX::DX11::GeometricPrimitive::VertexCollection vertices;
		DirectX::DX11::GeometricPrimitive::IndexCollection indices;
		DirectX::DX11::GeometricPrimitive::CreateTeapot(vertices, indices, 0.1f);

		TRawMeshData<CommonVertex, uint32_t> rawMeshData;

		for (size_t iVertex = 0; iVertex < vertices.size(); iVertex++)
		{
			auto& inVertex = vertices[iVertex];
			CommonVertex commonVertex;
			commonVertex.position = { inVertex.position.x, inVertex.position.y, inVertex.position.z, 1 };
			commonVertex.normal.x = inVertex.normal.x;
			commonVertex.normal.y = inVertex.normal.y;
			commonVertex.normal.z = inVertex.normal.z;
			commonVertex.textureCoordinate = inVertex.textureCoordinate;
			commonVertex.color = { 1, 0, 0, 1 };

			rawMeshData.vertices.push_back(commonVertex);
		}

		for (size_t iIndex = 0; iIndex < indices.size(); iIndex++) {
			rawMeshData.indices.push_back(indices[iIndex]);
		}

		auto meshCreator = DxMeshRenderDataCreator<CommonVertex, uint32_t>();
		auto hrResult = meshCreator.CreateDxMeshRenderData(poDevice, rawMeshData.vertices, rawMeshData.indices);

		return hrResult;
	}

	DxCommonMeshData DxMeshCreatorHelper::MakeGrid(ID3D11Device* poDevice, int linesPerAxis, float spacing)
	{
		std::vector<CommonVertex> vecVertex;
		std::vector<uint32_t> vecIndex;

		unsigned short index = 0;
		float c = 0.3f;
		for (int x = -linesPerAxis / 2; x < (linesPerAxis / 2) + 1; x++)
		{
			CommonVertex v1;
			v1.position = { x * spacing, 0, -(spacing * (linesPerAxis)) / 2, 0 };

			CommonVertex v2;
			v2.position = { x * spacing, 0, (spacing * (linesPerAxis)) / 2, 0 };

			v2.color = v1.color = { c, c, c, 1 };
			if (x == 0)  // make center line a different color
			{
				v2.color = v1.color = { 0.9f, 0.f, 0.f, 1 };
			}

			vecVertex.push_back(v1);
			vecIndex.push_back(index++);
			vecVertex.push_back(v2);
			vecIndex.push_back(index++);
		}

		for (int z = -linesPerAxis / 2; z < (linesPerAxis / 2) + 1; z++)
		{
			CommonVertex vertex1;
			vertex1.position = {
				(spacing * (linesPerAxis)) / 2,
				0,
				z * spacing,0
			};

			CommonVertex vertex2;
			vertex2.position = {
				-(spacing * (linesPerAxis)) / 2,
				0,
				z * spacing,0
			};

			vertex2.color = vertex1.color = { c, c, c, 1.f };

			if (z == 0) // make center line a different color
			{
				vertex2.color = vertex1.color = { 0, 0, 0.9f, 1 };

			}

			// -- store line
			vecVertex.push_back(vertex1);
			vecIndex.push_back(index++);
			vecVertex.push_back(vertex2);
			vecIndex.push_back(index++);
		}

		auto meshCreator = DxMeshRenderDataCreator<CommonVertex, uint32_t>();
		auto meshBuffer = meshCreator.CreateDxMeshRenderData(poDevice, vecVertex, vecIndex);

		meshBuffer.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

		return meshBuffer;
	}

	DxCommonMeshData DxMeshCreatorHelper::CreateFromRmv2Mesh(ID3D11Device* poDevice, const rmv2::MeshBlockCommon& rmv2Mesh)
	{
		auto meshCreator = DxMeshRenderDataCreator<CommonVertex, uint32_t>();

		// convert indices from uint16 to uint32
		std::vector<uint32_t> vecIndices32;
		for (auto index : rmv2Mesh.meshData.indices)
		{
			vecIndices32.push_back(index);
		}

		auto hrResult = meshCreator.CreateDxMeshRenderData(poDevice, rmv2Mesh.meshData.vertices, vecIndices32);

		return hrResult;
	}


	TRawMeshData<DirectX::XMFLOAT3, uint32_t> DxSkeletonMeshCreator::sm_cubeMeshData =
	{
		{
			{-1.0f, -1.0f, -1.0f},
			{1.0f, -1.0f, -1.0f},
			{1.0f, 1.0f, -1.0f},
			{-1.0f, 1.0f, -1.0f},
			{-1.0f, -1.0f, 1.0f},
			{1.0f, -1.0f, 1.0f },
			{1.0f, 1.0f, 1.0f},
			{-1.0f, 1.0f, 1.0f}
		},
		{
			0, 1,  1, 2,  2, 3,  3,0,   // side 1
			4, 5,  5, 6,  6, 7,  7,4,   // side 2

			0, 4,   1,5,   2,6,  3, 7  // connecting lines
		}
	};

	// Function to create vertices for a box mesh
	std::vector<DirectX::XMFLOAT3> createBoxVertices(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max) {
		return {
			{min.x, min.y, min.z},
			{max.x, min.y, min.z},
			{max.x, max.y, min.z},
			{min.x, max.y, min.z},
			{min.x, min.y, max.z},
			{max.x, min.y, max.z},
			{max.x, max.y, max.z},
			{min.x, max.y, max.z}
		};
	}

	// Function to create indices for a box mesh
	std::vector<uint32_t> createBoxIndices() {
		return {
			// Front face
			0, 1, 2, 2, 3, 0,
			// Back face
			4, 5, 6, 6, 7, 4,
			// Left face
			4, 7, 3, 3, 0, 4,
			// Right face
			1, 5, 6, 6, 2, 1,
			// Top face
			3, 2, 6, 6, 7, 3,
			// Bottom face
			4, 0, 1, 1, 5, 4
		};
	}

	DxCommonMeshData DxMeshCreatorHelper::MakeBoundingBoxMesh(ID3D11Device* poDevice, const DirectX::BoundingBox& bb, const sm::Color& color = { 1,1,1,1 })
	{
		auto vecPositions =
		{
			sm::Vector3(-1.0f, -1.0f, -1.0f) * bb.Extents + bb.Center,
			sm::Vector3(1.0f, -1.0f, -1.0f) * bb.Extents + bb.Center,
			sm::Vector3(1.0f, 1.0f, -1.0f) * bb.Extents + bb.Center,
			sm::Vector3(-1.0f, 1.0f, -1.0f) * bb.Extents + bb.Center,
			sm::Vector3(-1.0f, -1.0f, 1.0f) * bb.Extents + bb.Center,
			sm::Vector3(1.0f, -1.0f, 1.0f) * bb.Extents + bb.Center,
			sm::Vector3(1.0f, 1.0f, 1.0f) * bb.Extents + bb.Center,
			sm::Vector3(-1.0f, 1.0f, 1.0f) * bb.Extents + bb.Center
		};

		std::vector<CommonVertex> vecCommonVertices;
		for (const auto& pos : vecPositions)
		{
			CommonVertex newVertex;
			newVertex.position = { pos.x, pos.y, pos.z, 0.0f };
			newVertex.color = color;

			vecCommonVertices.push_back(newVertex);
		}

		std::vector<uint32_t> vecIndices =
		{
			0, 1, 1, 2, 2, 3, 3, 0,   // side 1
			4, 5, 5, 6, 6, 7, 7, 4,   // side 2

			0, 4, 1, 5, 2, 6, 3, 7  // connecting lines
		};

		auto meshCreator = DxMeshRenderDataCreator<CommonVertex, uint32_t>();
		auto meshBuffer = meshCreator.CreateDxMeshRenderData(poDevice, vecCommonVertices, vecIndices);
		meshBuffer.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

		return meshBuffer;
	}

	DxCommonMeshData Rmv2MeshCreator::CreateMesh(ID3D11Device* poDevice)
	{
		auto meshCreator = DxMeshRenderDataCreator<CommonVertex, uint32_t>();

		std::vector<uint32_t> vecIndices32;
		for (auto index : m_rmv2Mesh.meshData.indices) // convert indices from uint16 to uint32
		{
			vecIndices32.push_back(index);
		}

		auto hrResult = meshCreator.CreateDxMeshRenderData(poDevice, m_rmv2Mesh.meshData.vertices, vecIndices32);

		return hrResult;
	}
}