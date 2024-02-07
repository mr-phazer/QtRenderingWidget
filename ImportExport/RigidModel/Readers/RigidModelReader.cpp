#include "RigidModelReader.h"

#include "..\Creators\FileHeaderCreators.h"
#include "..\Creators\LodHeaderCreators.h"
#include "..\Creators\Vertices\VertexCreators.h"
#include "..\Creators\MeshHeaderType3Creator.h"
#include "..\Creators\MeshMaterialCreators.h"
#include "..\Creators\Material\IMaterialCommonCreators.h"

using namespace rmv2;

RigidModelFileCommon rmv2::RigidModelReader::Read(ByteStream& bytes)
{
	RigidModelFileCommon rmv2File;

	ReadFileHeaders(bytes, rmv2File);

	rmv2File.lods.resize(rmv2File.fileHeader.wLodCount);
	for (size_t lodIndex = 0; lodIndex < rmv2File.fileHeader.wLodCount; lodIndex++)
	{
		rmv2File.lods[lodIndex] =
			ReadModelBlock(bytes, rmv2File.fileHeader.modelVersionId, rmv2File.lodHeaders[lodIndex].dwMeshCount, lodIndex);
	}

	return rmv2File;
}

void rmv2::RigidModelReader::ReadFileHeaders(ByteStream& bytes, rmv2::RigidModelFileCommon& rmv2File)
{
	rmv2File.fileHeader = FileHeaderCreatorDefault().Create(bytes);
	rmv2File.lodHeaders = ReadLodHeaders(bytes, rmv2File.fileHeader.modelVersionId, rmv2File.fileHeader.wLodCount);
}

std::vector<LODHeaderCommon> rmv2::RigidModelReader::ReadLodHeaders(ByteStream& bytes, Rmv2VersionEnum rmv2VersionId, uint16_t wLODCount)
{
	auto lodHeaderCreator = m_lodHeaderCreatorFactory.Get(rmv2VersionId);

	std::vector<LODHeaderCommon> lodHeaders(wLODCount);

	for (auto& lodHeaderElement : lodHeaders)
	{
		lodHeaderElement = lodHeaderCreator->Create(bytes);
	}

	return lodHeaders;
}

ModelBlockCommon rmv2::RigidModelReader::ReadModelBlock(ByteStream& bytes, Rmv2VersionEnum rmv2Version, size_t meshCount, size_t lodIndex)
{
	ModelBlockCommon modelBlock;

	modelBlock.meshBlocks.resize(meshCount);

	for (size_t iMesh = 0; iMesh < meshCount; iMesh++)
	{
		modelBlock.meshBlocks[iMesh] = ReadMeshBlock(bytes, rmv2Version);
	}

	return modelBlock;
}

rmv2::MeshBlockCommon rmv2::RigidModelReader::ReadMeshBlock(ByteStream& bytes, Rmv2VersionEnum rmv2Version)
{
	MeshBlockCommon meshBlock;

	// get info on mesh
	meshBlock.meshHeader = MeshHeaderType3Creator().Create(bytes);

	auto materialCreator = m_materialCreatorFactory.Get(meshBlock.meshHeader.RigidMaterialId);
	meshBlock.materialBlock = materialCreator->Create(bytes);

	// TODO: make "IMeshCreator" too: (instead just reading...?) maybe?
	// -> auto meshCreator = m_meshCreatorFactory.Get(meshBlock.meshHeader.RigidMaterialId);
	ReadMeshData(bytes, meshBlock, rmv2Version);
	
	// TODO: remove if using the older method?
	//meshBlock.meshData = ReadMeshData(bytes, meshBlock.meshHeaderType3, meshBlock.meshHeaderType5);

	return meshBlock;
}

// TODO: REMOVE

void rmv2::RigidModelReader::ReadMeshData(ByteStream& bytes, rmv2::MeshBlockCommon& destMeshblock, Rmv2VersionEnum rmv2Version)
{
	auto vertexCreator = m_vertexCreatorProvider.Get(destMeshblock.materialBlock.materialHeader.VertexFormatId);

	destMeshblock.meshData.vertices.resize(destMeshblock.meshHeader.dwVertexCount);
	for (auto& vertex : destMeshblock.meshData.vertices)
	{
		vertex = vertexCreator->Create(bytes, rmv2Version);
	}

	destMeshblock.meshData.indices.resize(destMeshblock.meshHeader.dwIndexCount);
	for (auto& index : destMeshblock.meshData.indices)
	{
		index = bytes.GetElement<uint16_t>();
	}
}

// TODO: why do the vectors<vertex> etc not "exist" in debugger?

//MeshData16 rmv2::RigidModelReader::ReadMeshData_WEIRD(ByteStream& bytes, const MeshHeaderType3& meshHeader3, const MaterialHeaderType5& meshHeader5)
//{
//	auto vertexCreator = VertexCreatorFactory().Get(meshHeader5.VertexFormatId);
//
//	MeshData16 meshData;
//
//	meshData.vertices.resize(meshHeader3.dwVertexCount);
//	for (size_t iVertex = 0; iVertex < meshHeader3.dwVertexCount; iVertex++)
//	{
//		meshData.vertices[iVertex] = vertexCreator->Create(bytes.);
//	}
//
//	meshData.indices.resize(meshHeader3.dwIndexCount);
//	for (size_t iIndex = 0; iIndex < meshHeader3.dwIndexCount; iIndex++)
//	{
//		meshData.indices[iIndex] = bytes.GetElement<uint16_t>();
//	}
//
//	return meshData;
//}
