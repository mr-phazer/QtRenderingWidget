#pragma once

#include "IDxResource.h"
#include "Utils\ByteStream.h"

namespace rldx
{
	class DxResourceByteStream : public IDxResource
	{
	public:
		utils::ByteStream byteStream;
	};

}
