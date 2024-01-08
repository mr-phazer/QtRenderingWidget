#pragma once
#include "IDrawable.h"
#include "IBindable.h"

namespace Rldx {
	class IRenderQueueItem : public IDrawable, public IBindable {};
}