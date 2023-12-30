#pragma once
#pragma once

namespace Rldx {

	class IDxRenderQueue;	
	/// <summary>
	/// Base for anything that be draw with a shader program
	/// </summary>
	class IFlushable
	{
	public:
		virtual void FlushToRenderQueue(IDxRenderQueue* pRenderQueue) = 0;
	};
};