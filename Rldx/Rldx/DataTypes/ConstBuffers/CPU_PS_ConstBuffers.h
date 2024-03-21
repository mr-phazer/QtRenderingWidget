#pragma once

#include "Defs.h"

namespace rldx
{
	enum lightType : uint
	{
		DirectionalLight = 0,
		PointLight = 1,
		SpotLight = 2,
	};

	enum LightStateEnum : uint
	{
		Enabled = 1,
		Disabled = 0,
	};

	struct Light_Data
	{
		float3      position;
		float       reserved;

		float3      direction;
		float       reserved001;

		float4      color = { 1, 1, 1, 1 };

		float       SpotAngle;
		float       ConstantAttenuation;
		float       LinearAttenuation;
		float       QuadraticAttenuation;

		uint		lightType;
		uint		lighState;
		float		radiance = 1.0f;
		uint		reserved002;
	};

	/***************************************************************/
	// GPU side PIXEL SHADER constant buffers
	/***************************************************************/

	struct PS_Light_ConstBuffer
	{
		Light_Data lightData[1];

		float4 solidColor;

		float smoothness_curve;
		float reflect_curve;
		float reflections_roughness;
		float shadow_factor;

		float reserved001;
		uint use_AO;
		uint all_transparent;
		float exposure;

		float bloom_threshold;
		float bloom_intensity;
		int maskIndex;
		float reserved003;

		float4 screenRes;

		//float4 colorTable[3] = { {1,0,1,1}, {0,1,0,1}, {0,0,1,1} };
	};

	struct PS_AmbientLight_Data_ConstBuffer
	{
		float radiance = 0.0f;
		float roughness = 1.0f;
		float speculatFactor = 1.0f;
		float diffuseFactor = 1.0f;

		DirectX::XMFLOAT4X4 roatation = sm::Matrix::Identity;
		DirectX::XMFLOAT4 color = { 1, 1, 1, 1 };
	};

	/***************************************************************/
	// GPU side Vertex SHADER constant buffers
	/***************************************************************/
}