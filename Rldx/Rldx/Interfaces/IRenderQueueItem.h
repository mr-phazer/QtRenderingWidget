#pragma once
#include "IBindable.h"
#include "IDrawable.h"
#include "IUpdateable.h"

namespace rldx {
	class IRenderQueueItem : public IDrawable, public IBindable, public IUpdateable
	{

	};
}