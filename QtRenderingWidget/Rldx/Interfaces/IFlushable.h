#pragma once
#pragma once

namespace Rldx {

	class IRenderBucket;	
	/// <summary>
	/// Interface for objects that can be flushed to a render queue
	/// </summary>
	class IFlushable
	{
	public:
		virtual void FlushToRenderQueue(IRenderBucket* pRenderQueue) = 0;
	};
};