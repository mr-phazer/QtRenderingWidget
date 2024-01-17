#pragma once
#include "IDrawable.h"
#include "IBindable.h"

namespace rldx {
	class IRenderQueueItem : public IDrawable, public IBindable {};
}