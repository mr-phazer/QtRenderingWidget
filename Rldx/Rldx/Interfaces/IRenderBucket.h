#pragma once

namespace rldx {

	// -- forward declarations
	class IRenderQueueItem;

	class IRenderBucket
	{
	public:
		virtual void AddItem(IRenderQueueItem* item) = 0;
		virtual void ClearItems() = 0;
	};
}