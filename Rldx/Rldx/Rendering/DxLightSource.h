#pragma once

#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>

#include "..\Interfaces\IBindable.h"
#include "..\Interfaces\IUpdateable.h"
#include "..\DataTypes\ConstBuffers\CPUConstBuffers.h"
#include "DxConstBuffer.h"
#include "DxCameraOrbital.h"

namespace rldx
{
	class DxLightSource : public IBindable, public IUpdateable
	{
		TConstBuffer<PS_Light_ConstBuffer> m_lightConstBuffer;
		DxCameraOrbital* m_lightSourceController;
		bool m_bControlByCamera = true;

	public:
		void SetLightType(lightType type);
		void SetLightState(bool state);
		void SetControlledByCamera(bool state);
		void SetLightPosition(DirectX::SimpleMath::Vector3 position);;

		void SetLightDirection(DirectX::SimpleMath::Vector3 direction);;
		void SetLightRadiance(float radiance);
	};
}