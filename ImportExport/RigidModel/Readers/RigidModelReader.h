#pragma once

#include "..\DataTypes\RigidModelFile.h"
#include "..\Creators\LodHeaderCreators.h"
#include "..\Creators\Material\IMaterialCommonCreators.h"
#include "..\Creators\Vertices\VertexCreators.h"

namespace rmv2
{
	class RigidModelReader
	{
		LODHeaderCreatorFactory m_lodHeaderCreatorFactory;
		MaterialCommonCreatorFactory m_materialCreatorFactory;
		VertexCreatorProvider m_vertexCreatorProvider;

		// TODO: use this file to store data in? Before returning
		// RigidModelFileCommon m_rigidModelFile;

	public:
		RigidModelFileCommon Read(ByteStream& bytes);

		/// <summary>
		///  public to enable reading of header only by "client"
		/// </summary>		
		void ReadFileHeaders(ByteStream& bytes, rmv2::RigidModelFileCommon& rmv2File);

	private:
		std::vector<LODHeaderCommon> ReadLodHeaders(ByteStream& bytes, Rmv2VersionEnum rmv2VersionId, uint16_t wLODCount);
		
		ModelBlockCommon ReadModelBlock(ByteStream& bytes, Rmv2VersionEnum, size_t meshCount, size_t lodIndex);		
		rmv2::MeshBlockCommon ReadMeshBlock(ByteStream& bytes);
		
		// TODO: why do the vectors<vertex> etc not "exist" in debugger?
		MeshData16 ReadMeshData_WEIRD(ByteStream& bytes, const MeshHeaderType3& meshHeader3, const MeshHeader5Common& meshHeader5);

		void ReadMeshData(ByteStream& bytes, rmv2::MeshBlockCommon& meshblock);
		
	};
}
