#pragma once

#include <cstdint>

#include "IDataStructure.h"
#include <math.h>

namespace rmv2 {
		// make all variable into virtual methods retuning references, dont use  "" sm_prefix

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

		struct LodHeaderData_V6
		{
			uint32_t dwMeshCount = 0;
			uint32_t dwVerticesDataLength = 0;
			uint32_t dwIndicesDataLength = 0;
			uint32_t dwStartOffset = 0;
			float fVisibilityDistance = 100.0f;
			uint32_t dwAuthoredLodNumber = 0;
			uint32_t dwQualityLevel = 0;
		};
		
		struct LODHeaderV6 : public ILODHeader
		{
			uint32_t& MeshCount() override { return data.dwMeshCount; };
			uint32_t& VerticesDataLength() override { return data.dwVerticesDataLength; };
			uint32_t& IndicesDataLength() override { return data.dwIndicesDataLength; };
			uint32_t& StartOffset() override { return data.dwAuthoredLodNumber; };
			float& VisibilityDistance() override { return data.fVisibilityDistance; };
			uint32_t& AuthoredLodNumber() override { return data.dwAuthoredLodNumber; };
			uint32_t& QualityLevel() override { return data.dwQualityLevel; };			

			LodHeaderData_V6 data;		
		};

		struct LODHeaderCommon : IDataStructure
		{
			bool IsContentValid()  override
			{
				// TODO: check if 100 is an ok max vaue
				return
					(dwMeshCount < 100) && // 100 = just a randomly picked max. 
					(!isnan(fVisibilityDistance)); // float being a NAN meaning corrupted data read
			}


			uint32_t dwMeshCount = 0;
			uint32_t dwVerticesDataLength = 0;
			uint32_t dwIndicesDataLength = 0;
			uint32_t dwStartOffset = 0;
			float fVisibilityDistance = 100.0f;
			uint32_t dwAuthoredLodNumber = 0;
			
			struct QualityLevel
			{
				std::uint8_t byte0;
				std::uint8_t byte1;
				std::uint8_t byte2;
				std::uint8_t byte3;
			}
			qualityLevel;
		};

}; // namespace rmv2