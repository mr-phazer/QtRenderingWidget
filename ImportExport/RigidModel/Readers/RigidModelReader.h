#pragma once

#include "..\DataTypes\RigidModelFile.h"
#include "..\Creators\FileHeaderCreators.h"
#include "..\Creators\LodHeaderCreators.h"
#include "..\Creators\VertexCreators\VertexCreators.h"
#include "..\Creators\MeshHeaderType3Creator.h"
#include "..\Creators\MeshHeaderType5Creator.h"
#include "..\Creators\Material\MaterialBlockCommonCreators.h"


namespace rmv2
{
	class RigidModelReader
	{
	public:
		RigidModelFileCommon Read(ByteStream& bytes)
		{
			RigidModelFileCommon rmv2File;

			ReadHeaders(bytes, rmv2File);

			rmv2File.lods.resize(rmv2File.fileHeader.wLodCount);
			for (size_t lodIndex = 0; lodIndex < rmv2File.fileHeader.wLodCount; lodIndex++)
			{			
				rmv2File.lods[lodIndex] =
					ReadModelBlock(bytes, rmv2File.fileHeader.modelVersionId, rmv2File.lodHeaders[lodIndex].dwMeshCount, lodIndex);
			}

			return rmv2File;
		}

		void ReadHeaders(ByteStream& bytes, rmv2::RigidModelFileCommon& rmv2File)
		{
			rmv2File.fileHeader = FileHeaderCreatorDefault().Create(bytes);
			rmv2File.lodHeaders = ReadLodHeaders(bytes, rmv2File.fileHeader.modelVersionId, rmv2File.fileHeader.wLodCount);
		}
			

	private:
		std::vector<LODHeaderCommon> ReadLodHeaders(ByteStream& bytes, Rmv2VersionEnum rmv2VersionId, uint16_t wLODCount)
		{
			auto lodHeaderCreator = LODHeaderCreatorFactory().Get(rmv2VersionId);

			std::vector<LODHeaderCommon> lodHeaders(wLODCount);

			for (auto& lodHeaderElement : lodHeaders)
			{
				lodHeaderElement = lodHeaderCreator->Create(bytes);
			}			

			return lodHeaders;
		}

		ModelBlockCommon ReadModelBlock(ByteStream& bytes, Rmv2VersionEnum, size_t meshCount, size_t lodIndex)
		{
			ModelBlockCommon modelBlock;

			modelBlock.meshBlocks.resize(meshCount);

			for (size_t iMesh = 0; iMesh < meshCount; iMesh++)
			{ 
				modelBlock.meshBlocks[iMesh] = ReadMeshBlock(bytes);				
			}

			return modelBlock;
		}

		rmv2::MeshBlockCommon ReadMeshBlock(ByteStream& bytes)
		{
			MeshBlockCommon meshBlock;

			meshBlock.meshHeaderType3 = MeshHeaderType3Creator().Create(bytes);
			meshBlock.meshHeaderType5 = MeshHeaderType5Creator().Create(bytes);
			meshBlock.materialBlock = MaterialBlockCommonCreator().Create(bytes, meshBlock.meshHeaderType5);

			meshBlock.meshData = ReadMeshData(bytes, meshBlock.meshHeaderType3, meshBlock.meshHeaderType5);

			return meshBlock;
		}

		// TODO: REMOVE
		void ReadMeshData_OLD(ByteStream& bytes, rmv2::MeshBlockCommon& meshblock)
		{
			auto vertexCreator = VertexCreatorProvider().Get(meshblock.meshHeaderType5.VertexFormatId);

			meshblock.meshData.vertices.resize(meshblock.meshHeaderType3.dwVertexCount);
			for (auto& vertex : meshblock.meshData.vertices)
			{
				vertex = vertexCreator->Create(bytes);
			}

			meshblock.meshData.indices.resize(meshblock.meshHeaderType3.dwIndexCount);
			for (auto& index : meshblock.meshData.indices)
			{
				index = bytes.GetElement<uint16_t>();
			}
		}

		MeshData16 ReadMeshData(ByteStream& bytes, const MeshHeaderType3& meshHeader3, const MeshHeaderType5& meshHeader5)
		{			
			auto vertexCreator = VertexCreatorProvider().Get(meshHeader5.VertexFormatId);
			
			MeshData16 mesh;
			mesh.vertices.resize(meshHeader3.dwVertexCount);

			for (size_t iVertex = 0; iVertex < meshHeader3.dwVertexCount; iVertex++)
			{			
				mesh.vertices[iVertex] = vertexCreator->Create(bytes);
			}

			mesh.indices.resize(meshHeader3.dwIndexCount);
			for (size_t iIndex = 0; iIndex < meshHeader3.dwIndexCount; iIndex++)
			{
				mesh.indices[iIndex] = bytes.GetElement<uint16_t>();
			}
			
			return mesh;
		}
	};
}
