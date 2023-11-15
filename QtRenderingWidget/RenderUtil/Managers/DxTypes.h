#pragma once

#include <string>

using NodeId = uint32_t;
using ResourceId = uint32_t;

enum class ResourceTypeEnum : uint32_t
{
	Unknown = 0,
	Mesh,
	Camera,
	Light,
	Skybox,
	ParticleSystem,
	Sprite,
	Sound,
	Material,
	Texture,
	Shader,
	Count
};

