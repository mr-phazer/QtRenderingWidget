#pragma once
#include "IBindable.h"
#include "IDrawable.h"
#include "IFlushable.h"
#include "IUpdateable.h"

namespace rldx {
	class IRenderQueueItem : public IDrawable, public IBindable, public IUpdateable
	{

	};
}