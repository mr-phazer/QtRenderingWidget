#include "DxMeshCreator.h"



#include "..\..\..\DirectXTK\Inc\GeometricPrimitive.h"



// TODO: remove once tested
Rldx::DxCommonMeshData Rldx::MakeTestCubeMesh(ID3D11Device* poDevice)
{
	DirectX::DX11::GeometricPrimitive::VertexCollection vertices;
	DirectX::DX11::GeometricPrimitive::IndexCollection indices;
	DirectX::DX11::GeometricPrimitive::CreateTeapot(vertices, indices);

	TRawMeshData<Rldx::CommonVertex, uint32_t> rawMeshData;

	for (size_t iVertex = 0; iVertex < vertices.size(); iVertex++)
	{
		auto& inVertex = vertices[iVertex];
		CommonVertex commonVertex;
		commonVertex.position = { inVertex.position.x, inVertex.position.y, inVertex.position.z, 1 };
		commonVertex.normal.x = -inVertex.normal.x;
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
};
