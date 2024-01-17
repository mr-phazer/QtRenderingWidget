#include "DxLightSource.h"

void rldx::DxLightSource::SetLightType(lightType type)
{
	m_lightConstBuffer.data.lightType = type;
}

inline void rldx::DxLightSource::SetLightState(bool state)
{
	m_lightConstBuffer.data.lighState = LightStateEnum(state);
}

inline void rldx::DxLightSource::SetControlledByCamera(bool state)
{
	m_bControlByCamera = state;
}

inline void rldx::DxLightSource::SetLightPosition(DirectX::SimpleMath::Vector3 position)
{
	if (m_bControlByCamera)
		m_lightSourceController->SetEyePosition(position);
	else
		m_lightConstBuffer.data.position = { position.x, position.y, position.z, 0 };
}

inline void rldx::DxLightSource::SetLightDirection(DirectX::SimpleMath::Vector3 direction)
{
	if (m_bControlByCamera)
		m_lightSourceController->SetLookAt(direction);
	else
		m_lightConstBuffer.data.direction = { direction.x, direction.y, direction.z, 0 };
}

inline void rldx::DxLightSource::SetLightRadiance(float radiance)
{
	m_lightConstBuffer.data.radiance = radiance;
}
