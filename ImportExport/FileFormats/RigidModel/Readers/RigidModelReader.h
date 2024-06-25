#pragma once

#include "..\Creators\LodHeaderCreators.h"
#include "..\Creators\Material\IMaterialCommonCreators.h"
#include "..\Creators\Vertices\VertexCreators.h"
#include "..\Types\Common\RigidModelFile.h"

namespace rmv2
{
	class RigidModelReader
	{
		LODHeaderCreatorFactory m_lodHeaderCreatorFactory;
		MaterialCommonCreatorFactory m_materialCreatorFactory;
		VertexCreatorFactory m_vertexCreatorProvider;

	public:
		RigidModelFileCommon Read(ByteStream& bytes);

	private:
		void ReadFileHeaders(ByteStream& bytes, rmv2::RigidModelFileCommon& rmv2File);

		std::vector<LODHeaderCommon> ReadLodHeaders(ByteStream& bytes, Rmv2VersionEnum rmv2VersionId, uint16_t wLODCount);

		ModelBlockCommon ReadModelBlock(ByteStream& bytes, Rmv2VersionEnum, size_t meshCount, size_t lodIndex);

		rmv2::MeshBlockCommon ReadMeshBlock(ByteStream& bytes, Rmv2VersionEnum rmv2Version);

		void ReadMeshData(ByteStream& bytes, rmv2::MeshBlockCommon& meshblock, Rmv2VersionEnum rmv2Version);

	};
};
