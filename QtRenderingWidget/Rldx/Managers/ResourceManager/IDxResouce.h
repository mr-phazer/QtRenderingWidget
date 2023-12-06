#pragma once

namespace Rldx {

	enum ResourceTypeEnum : uint32_t
	{
		Unknown,
		Texture,
		Shader,
		Mesh,
		Material
	};

	class IDxResource
	{
	public:
		virtual ResourceTypeEnum GetType() const = 0;

	};
}; // namespce Rldx