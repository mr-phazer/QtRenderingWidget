#include "DxMeshCreator.h"



#include "..\..\..\DirectXTK\Inc\GeometricPrimitive.h"
#include "..\Types\ConstBuffers\CPUConstBuffers.h"



// TODO: remove once tested
Rldx::DxCommonMeshData Rldx::ModelCreator::MakeTestCubeMesh(ID3D11Device* poDevice)
{
	DirectX::DX11::GeometricPrimitive::VertexCollection vertices;
	DirectX::DX11::GeometricPrimitive::IndexCollection indices;
	DirectX::DX11::GeometricPrimitive::CreateTeapot(vertices, indices, 0.1);

	TRawMeshData<Rldx::CommonVertex, uint32_t> rawMeshData;

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

Rldx::DxCommonMeshData Rldx::ModelCreator::MakeGrid(ID3D11Device* poDevice, int linesPerAxis, float spacing)
{
	std::vector<CommonVertex> vecVertex;
	std::vector<uint32_t> vecIndex;

	unsigned short index = 0;
	float c = 1.0f;
	for (int x = -linesPerAxis / 2; x < (linesPerAxis / 2) + 1; x++)
	{
		CommonVertex v1;
		//v1.position = { x * spacing, 0, -( spacing * ( linesPerAxis - 1 ) ) / 2   , 1 };
		v1.position = { x * spacing, 0, -(spacing * (linesPerAxis)) / 2, 0 };

		//	0	float c = 0.5f * (x + ((float)linesPerAxis / 2.f)) / (float)linesPerAxis;

		v1.color = { c, c, c, 1 };
		v1.color = { 1, 1, 1, 1 };

		if (x == 0)  // make center line a different color
		{
			v1.color = { 0.9, 0.1,  0.1, 1 };
		}

		CommonVertex v2;
		//v2.position = { x * spacing, 0, ( spacing * ( linesPerAxis - 1 ) ) / 2    , 1 };
		v2.position = { x * spacing, 0, (spacing * (linesPerAxis)) / 2, 0 };
		v2.color = { c, c, c, 1 };

		v2.color = { 1, 1, 1, 1 };

		vecVertex.push_back(v1);
		vecIndex.push_back(index++);
		vecVertex.push_back(v2);
		vecIndex.push_back(index++);
	}

	for (int z = -linesPerAxis / 2; z < (linesPerAxis / 2) + 1; z++)
	{
		CommonVertex v1;
		v1.position = {
			(spacing * (linesPerAxis)) / 2,
			0,
			z * spacing,0
		};

		//oat c = 0.5f * (z + ((float)linesPerAxis / 2.f)) / (float)linesPerAxis;
		float c = 1.0f;


		v1.color = { c, c, c, 1 };
		if (z == 0) // make center line a different color
		{
			v1.color = { 0, 0, 0.9, 1 };
		}

		CommonVertex v2;
		v2.position = {
			-(spacing * (linesPerAxis)) / 2,
			0,
			z * spacing,0
		};

		//c = (z + ((float)linesPerAxis / 2.f)) / (float)linesPerAxis;
		c = 0.1f;
		v2.color = { 1, 1, 1, 1 };
		v1.color = { 1, 1, 1, 1 };
		//if (z == 0)
		//{
		//	v2.color = { 1.0, 0, 0, 1 };
		//}

		vecVertex.push_back(v1);
		vecIndex.push_back(index++);
		vecVertex.push_back(v2);
		vecIndex.push_back(index++);
	}

	auto meshCreator = DxMeshDataCreator<CommonVertex, uint32_t>();
	auto result = meshCreator.CreateDxMeshData(poDevice, vecVertex, vecIndex);

	result.enumTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	return result;
}


//static Rldx::DxCommonMeshData GenerateGrid(ID3D11Device* poDevice, size_t linesPerAxis)
//{
//	/*getLodMeshes().lods().resize(1);
//	getLodMeshes()[0].resize(1);*/
//
//
//	
//	std:::vector <CommonVertex> vecVertex;
//	std::vector <unsigned short> vecIndex;
//
//	unsigned short index = 0;
//	float c = 1.0f;
//	for (int x = -linesPerAxis / 2; x < (linesPerAxis / 2) + 1; x++)
//	{
//		CommonVertex v1;
//		//v1.position = { x * spacing, 0, -( spacing * ( linesPerAxis - 1 ) ) / 2   , 1 };
//		v1.position = { x * spacing, 0, -(spacing * (linesPerAxis - 1)) / 2, 0 };
//
//		//	0	float c = 0.5f * (x + ((float)linesPerAxis / 2.f)) / (float)linesPerAxis;
//
//		v1.color = { c, c, c, 1 };
//
//		if (x == 0)  // make center line a different color
//		{
//			v1.color = { 0.9, 0.1,  0.1, 1 };
//		}
//
//		CommonVertex v2;
//		//v2.position = { x * spacing, 0, ( spacing * ( linesPerAxis - 1 ) ) / 2    , 1 };
//		v2.position = { x * spacing, 0, (spacing * (linesPerAxis - 1)) / 2, 0 };
//		v2.color = { c, c, c, 1 };
//
//		vecVertex.push_back(v1);
//		vecIndex.push_back(index++);
//		vecVertex.push_back(v2);
//		vecIndex.push_back(index++);
//	}
//
//	for (int z = -linesPerAxis / 2; z < (linesPerAxis / 2) + 1; z++)
//	{
//		CommonVertex v1;
//		v1.position = {
//			(spacing * (linesPerAxis - 1)) / 2,
//			0,
//			z * spacing,0
//		};
//
//		//oat c = 0.5f * (z + ((float)linesPerAxis / 2.f)) / (float)linesPerAxis;
//		float c = 0.1f;
//
//
//		v1.color = { c, c, c, 1 };
//		if (z == 0) // make center line a different color
//		{
//			v1.color = { 0, 0, 0.9, 1 };
//		}
//
//		CommonVertex v2;
//		v2.position = {
//			-(spacing * (linesPerAxis - 1)) / 2,
//			0,
//			z * spacing,0
//		};
//
//		//c = (z + ((float)linesPerAxis / 2.f)) / (float)linesPerAxis;
//		c = 0.1f;
//		v2.color = { c, c, c, 1 };
//
//		//if (z == 0)
//		//{
//		//	v2.color = { 1.0, 0, 0, 1 };
//		//}
//
//		vecVertex.push_back(v1);
//		vecIndex.push_back(index++);
//		vecVertex.push_back(v2);
//		vecIndex.push_back(index++);
//	}
//
//	spoGridMeshData->createVertexBuffer(_poScene->getDevice(), (UINT)vecVertex.size() * sizeof(CommonVertex), &vecVertex[0]);
//	spoGridMeshData->createIndexBuffer(_poScene->getDevice(), (UINT)vecIndex.size() * 2, &vecIndex[0]);
//
//	spoGridMeshData->setIndexCount((UINT)vecIndex.size());
//
//	spoGridMeshData->EIndexFormat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
//	spoGridMeshData->ETopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
//
//	/*spoGridMeshData->m_poPixelShader =
//		_poScene->getResources()->getPixelShader(Resources::EPixelShaders::eSolidColor);
//
//	spoGridMeshData->m_poVertexShader =
//		_poScene->getResources()->getVertexShader(Resources::EVertexShader::eSimple);
//
//	spoGridMeshData->m_poInputLayout =
//		_poScene->getResources()->getInputLayout(Resources::EVertexShader::eSimple);*/
//
//
//
//	return true;
//}
//;
