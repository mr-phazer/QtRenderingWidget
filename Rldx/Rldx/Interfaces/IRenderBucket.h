#pragma once

namespace rldx {

	// -- forward declarations
	struct DxMeshRenderingData;

	class IRenderBucket
	{
	public:
		virtual void AddItem(DxMeshRenderingData* item) = 0;
		virtual void ClearItems() = 0;
	};
}