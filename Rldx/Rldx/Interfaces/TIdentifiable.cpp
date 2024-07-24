#include "TIdentifiable.h"

namespace rldx {

	IntId IdentifiableBase::sm_nextId = 1;
	IntId UniqueIdGenerator::sm_nextId = 1;

} // namespace rldx