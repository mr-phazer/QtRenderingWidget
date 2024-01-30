#include "DxTextureSamplers.h"

rldx::DxTextureSamplers rldx::DxTextureSamplers::Create(DirectX::CommonStates& commonStates)
{
	DxTextureSamplers newInstance;
	newInstance.samplers =
	{
		commonStates.LinearWrap(),  // 0 
		commonStates.AnisotropicWrap(), // 1
		commonStates.PointClamp(), // 2

		commonStates.LinearWrap(), // 3
		commonStates.LinearWrap(), // 4

		commonStates.PointClamp(), // 5
		commonStates.AnisotropicClamp() // 6
	};

	return newInstance;
}
