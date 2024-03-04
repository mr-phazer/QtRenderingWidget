#pragma once

#include <cstdint>

namespace rmv2 {
	namespace raw_structs {	

		struct LodHeaderElement_V5_V6
		{
			uint32_t dwMeshCount = 0;
			uint32_t dwVerticesDataLength = 0;
			uint32_t dwIndicesDataLength = 0;
			uint32_t dwStartOffset = 0;
			float fVisibilityDistance = 0.0f;		// distance when engine switches to next LOD
		};		

		struct LodHeaderElement_V7_V8
		{
			uint32_t dwMeshCount = 0;
			uint32_t dwVerticesDataLength = 0;
			uint32_t dwIndicesDataLength = 0;
			uint32_t dwStartOffset = 0;
			float fVisibilityDistance = 0.0f;		// distance when engine switches to next LOD

			uint32_t dwAuthoredLodNumber = 0;					// used in v8 and does some sort of counting of LOD headers, as LOD_HEADER[0].u1 = 0, LOD_HEADER[1].u1 = 1, and goes on like that
			//LodQualityLevel_V7_V8 oQualityLevel; // v7/v8 related to graphics setting "mesh_index quality"
 
			// TODO: internal struct? Written like this so examine the bytes in the debugger
			struct LodQualityLevel_V7_V8 // TODO: at some point, find out what each of the bytes actually do/mean?
			{
				uint8_t byte1 = 0;
				uint8_t byte2 = 0;
				uint8_t byte3 = 0;
				uint8_t byte4 = 0;
			}
			qualityLevelInfo;
		};
	}
}