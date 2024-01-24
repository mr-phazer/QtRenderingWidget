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
		float       reversed1;

		float4      color  = { 1, 1, 1, 1 };

		float       SpotAngle;
		float       ConstantAttenuation;
		float       LinearAttenuation;
		float       QuadraticAttenuation;

		uint		lightType;
		uint		lighState;
		float		radiance = 1.0f;
		uint		reserved1;
	};

	/***************************************************************/
	// GPU side PIXEL SHADER constant buffers
	/***************************************************************/

	struct PS_Light_ConstBuffer
	{
		Light_Data lightData[10];

		float4 env_color;
		float4x4 mEnv;

		float4 solidColor;

		float smoothness_curve;
		float reflect_curve;
		float reflections_roughness;
		float shadow_factor;

		float env_light_radiance;
		uint use_AO;
		uint all_transparent;
		float exposure;

		float bloom_threshold;
		float bloom_intensity;
		int maskIndex;
		float reserved___PS_LightBuffer_2;

		float4 screenRes;

		float4 colorTable[3] = { {1,0,0,1}, {0,1,0,1}, {0,0,1,1} };
	};

	struct PS_AmbientLight_Data_ConstBuffer
	{
		float radiance = 1.0;
		float roughness = 1.0;
		float speculatFactor = 1.0;
		float diffuseFactor = 1.0;

		DirectX::XMFLOAT4X4 roatation = sm::Matrix::Identity;
		DirectX::XMFLOAT4 color;
	};

	/***************************************************************/
	// GPU side Vertex SHADER constant buffers
	/***************************************************************/
}