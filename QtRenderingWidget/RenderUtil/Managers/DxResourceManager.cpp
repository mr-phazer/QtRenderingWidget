#include "DxResourceManager.h"

using namespace Rldx;

std::unique_ptr<Rldx::ResourceManager> Rldx::ResourceManager::m_spuInstance;



uint64_t IntAutoKeyBase::sm_nextId;