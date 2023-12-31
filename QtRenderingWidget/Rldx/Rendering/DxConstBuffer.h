#pragma once

#pragma once

#include <DirectXMath.h>
#include <SimpleMath.h>

#include "..\..\DirectXTK\Inc\BufferHelpers.h"

namespace Rldx {

	template<typename DATA_TYPE>
	struct TConstBuffer
	{
		/// <summary>
		/// Data, typically a struct, to be copied to the GPU
		/// Stored on and editable from the CPU
		/// </summary>
		DATA_TYPE data;

		/// <summary>
		/// Wraps GPU data buffer (MS DirectXTK class
		/// </summary>
		DirectX::ConstantBuffer<DATA_TYPE> buffer; 

		void CopyDataToGPU(ID3D11DeviceContext* poDeviceContext)
		{
			buffer.SetData(poDeviceContext, data);
		}
	};	
};

