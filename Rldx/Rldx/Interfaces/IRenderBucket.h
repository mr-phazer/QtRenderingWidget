#pragma once

namespace rldx {

	// -- forward declarations
	struct DxMeshRenderingData;

	// TODO: rename to "queue" or somemore more descriptive
	class IRenderBucket
	{
	public:
		virtual void AddItem(DxMeshRenderingData* item) = 0;
		virtual void ClearItems() = 0;
	};
}