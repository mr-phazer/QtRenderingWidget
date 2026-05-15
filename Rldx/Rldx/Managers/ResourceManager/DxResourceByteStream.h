#pragma once

#include "IDxResource.h"
#include <CommonLibs/Utils/ByteStream.h>

namespace rldx
{
	class DxResourceByteStream : public IDxResource
	{
		friend class ResourceManager;
	public:
		utils::ByteStream byteStream;
	};

}
