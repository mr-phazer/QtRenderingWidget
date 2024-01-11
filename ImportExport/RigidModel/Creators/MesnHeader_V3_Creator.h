#pragma once

#include "..\DataTypes\MeshHeader_V3.h"

namespace rmv2 {
	namespace mesh_header {		
		
		class MeshHeader_Type3_Creator_V6_V7_V8 : public IMeshHeader_Type3_Creator
		{			
		public:
			MeshHeader_Type3_Common Create(ByteStream& bytes) override;
		};

	}; // namespace file_header

}; // namespace rmv2


