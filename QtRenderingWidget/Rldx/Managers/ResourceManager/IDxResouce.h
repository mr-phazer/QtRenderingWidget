#pragma once

namespace Rldx {

	enum SceneNodeTypeEnum : uint32_t
	{
		Unknown,
		EmptyNode,
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