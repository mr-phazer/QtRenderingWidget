#include "DxMeshCreator.h"



#include "..\..\..\DirectXTK\Inc\GeometricPrimitive.h"
#include "..\DataTypes\CommonVertex.h"



// TODO: remove once tested
rldx::DxCommonMeshData rldx::DxMeshCreatorHelper::MakeTestCubeMesh(ID3D11Device* poDevice)
{
	DirectX::DX11::GeometricPrimitive::VertexCollection vertices;
	DirectX::DX11::GeometricPrimitive::IndexCollection indices;
	DirectX::DX11::GeometricPrimitive::CreateTeapot(vertices, indices, 0.1);

	TRawMeshData<rldx::CommonVertex, uint32_t> rawMeshData;

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
	
	auto meshCreator = DxMeshDataCreator<CommonVertex, uint32_t>();
	auto result = meshCreator.CreateDxMeshData(poDevice, rawMeshData.vertices, rawMeshData.indices);

	return result;
}

rldx::DxCommonMeshData rldx::DxMeshCreatorHelper::MakeGrid(ID3D11Device* poDevice, int linesPerAxis, float spacing)
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
			v2.color = v1.color = { 0.9, 0, 0, 1 };			
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

		vertex2.color = vertex1.color = { c, c, c, 1 };
	
		if (z == 0) // make center line a different color
		{
			vertex2.color = vertex1.color = { 0, 0, 0.9, 1 };
		
		}

		// -- store line
		vecVertex.push_back(vertex1);
		vecIndex.push_back(index++);
		vecVertex.push_back(vertex2);
		vecIndex.push_back(index++);
	}

	auto meshCreator = DxMeshDataCreator<CommonVertex, uint32_t>();
	auto result = meshCreator.CreateDxMeshData(poDevice, vecVertex, vecIndex);

	result.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	return result;
}

rldx::DxCommonMeshData rldx::DxMeshCreatorHelper::CreateFromRmv2Mesh(ID3D11Device* poDevice, const rmv2::MeshBlockCommon& rmv2Mesh)
{
	auto meshCreator = DxMeshDataCreator<CommonVertex, uint32_t>();
	
	std::vector<uint32_t> vecIndices32;
	for (auto index : rmv2Mesh.meshData.indices)
	{
		vecIndices32.push_back(index);
	}
	
	auto result = meshCreator.CreateDxMeshData(poDevice, rmv2Mesh.meshData.vertices, vecIndices32);

	return result;
}
