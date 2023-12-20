#pragma once

namespace Rldx {

	enum SceneNodeTypeEnum : uint32_t
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
		virtual SceneNodeTypeEnum GetType() const = 0;

	};
}; // namespce Rldx