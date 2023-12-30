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
		/// CPU Asssessible data
		/// </summary>
		DATA_TYPE data;

		/// <summary>
		/// Wraps GPU data buffer
		/// </summary>
		DirectX::ConstantBuffer<DATA_TYPE> buffer; // constant buffer for the vertex shader

		void CopyDataToGPU(ID3D11DeviceContext* poDeviceContext)
		{
			buffer.SetData(poDeviceContext, data);
		}
	};	
};

