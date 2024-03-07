#pragma once

#include "..\Types\Common\MeshHeaderType3.h"
#include "..\Types\Common\MeshHeaderType5.h"

namespace rmv2
{
	// make virtual functions for all data entries
	struct MeshHeader { 			

		// write virtual functions for all data entries the meshheaders
		virtual uint32_t& GetVertexCount() { return meshHeader3.dwVertexCount; };

	



		MeshHeaderType3 meshHeader3;
		MaterialHeaderType5 meshHeader5;

	};
}