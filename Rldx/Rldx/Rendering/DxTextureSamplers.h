#pragma once

#include <vector>
#include <CommonStates.h>

#include "..\Interfaces\IBindable.h"

namespace rldx {

	class DxTextureSamplers : IBindable
	{
		std::vector<ID3D11SamplerState*> samplers;

	public:
		DxTextureSamplers() = default;
		static DxTextureSamplers Create(DirectX::CommonStates& commonStates);

		void BindToDC(ID3D11DeviceContext* poDC) override
		{
			poDC->PSSetSamplers(0, static_cast<UINT>(samplers.size()), samplers.data());
		}
	};

} // namespace rldx