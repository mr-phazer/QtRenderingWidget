#pragma once

#include "..\..\Helpers\ByteStream.h"
#include "..\..\Helpers\TFactory.h"
#include "FileHeader.h"

using ByteStream = file_helpers::ByteStream;

namespace rmv2 {
	namespace lod_header {

		struct LODHeaderData
		{
			uint32_t m_dwMeshCount = 0;
			uint32_t m_dwVerticesDataLength = 0;
			uint32_t m_dwIndicesDataLength = 0;
			uint32_t m_dwStartOffset = 0;
			float m_fVisibilityDistance = 100.0f;
			uint32_t m_dwAuthoredLodNumber = 0;
			uint32_t m_dwQualityLevel = 0;
		};

		// make all variable into virtual methods retuning references, dont use  "" prefix

		struct ILODHeader
		{
			virtual uint32_t& MeshCount() = 0;
			virtual uint32_t& VerticesDataLength() = 0;
			virtual uint32_t& IndicesDataLength() = 0;
			virtual uint32_t& StartOffset() = 0;
			virtual float& VisibilityDistance() = 0;
			virtual uint32_t& AuthoredLodNumber() = 0;
			virtual uint32_t& QualityLevel() = 0;
		};

		struct LODHeader : public ILODHeader
		{
			uint32_t& MeshCount() override { return Data.m_dwMeshCount; };
			uint32_t& VerticesDataLength() override { return Data.m_dwVerticesDataLength; };
			uint32_t& IndicesDataLength() override { return Data.m_dwIndicesDataLength; };
			uint32_t& StartOffset() override { return Data.m_dwAuthoredLodNumber; };
			float& VisibilityDistance() override { return Data.m_fVisibilityDistance; };
			uint32_t& AuthoredLodNumber() override { return Data.m_dwAuthoredLodNumber; };
			uint32_t& QualityLevel() override { return Data.m_dwQualityLevel; };

			LODHeaderData Data;
		};

		struct LODHeaderCommon
		{
			uint32_t m_dwMeshCount = 0;
			uint32_t m_dwVerticesDataLength = 0;
			uint32_t m_dwIndicesDataLength = 0;
			uint32_t m_dwStartOffset = 0;
			float m_fVisibilityDistance = 100.0f;
			uint32_t m_dwAuthoredLodNumber = 0;
			uint32_t m_dwQualityLevel = 0;
		};

		class ILODHeaderCreator
		{
		public:
			virtual LODHeaderCommon Create(ByteStream& bytes) = 0;
		};		

	}; // namespace lod_header

}; // namespace rmv2