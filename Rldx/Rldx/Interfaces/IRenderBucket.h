#pragma once

namespace rldx {

	// -- forward declarations
	struct DxMeshData;

	class IRenderBucket
	{
	public:
		virtual void AddItem(DxMeshData* item) = 0;
		virtual void ClearItems() = 0;
	};
}