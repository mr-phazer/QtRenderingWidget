#pragma once

#include "..\DataTypes\MeshHeaderType3.h"
#include "..\DataTypes\MeshHeaderType5.h"

namespace rmv2
{
	// make virtual functions for all data entries
	struct MeshHeader { 			

		// write virtual functions for all data entries the meshheaders
		virtual uint32_t& GetVertexCount() { return meshHeader3.dwVertexCount; };

	



		MeshHeaderType3 meshHeader3;
		MeshHeader5Common meshHeader5;

	};
}