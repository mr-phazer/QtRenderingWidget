#include "DxLightSource.h"

void rldx::DxLightSource::SetLightType(lightType type)
{
	m_lightConstBuffer.data.lightData[0].lightType = type;
}

inline void rldx::DxLightSource::SetLightState(bool state)
{
	m_lightConstBuffer.data.lightData[0].lighState = LightStateEnum(state);
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
		m_lightConstBuffer.data.lightData[0].position = { position.x, position.y, position.z };
}

inline void rldx::DxLightSource::SetLightDirection(DirectX::SimpleMath::Vector3 direction)
{
	if (m_bControlByCamera)
		m_lightSourceController->SetLookAt(direction);
	else
		m_lightConstBuffer.data.lightData[0].direction = { direction.x, direction.y, direction.z };
}

inline void rldx::DxLightSource::SetLightRadiance(float radiance)
{
	m_lightConstBuffer.data.lightData[0].radiance = radiance;
}
