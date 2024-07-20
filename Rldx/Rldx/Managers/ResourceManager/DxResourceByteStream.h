#pragma once

#include "IDxResource.h"
#include <CommonLibs/Utils/ByteStream.h>

namespace rldx
{
	class DxResourceByteStream : public IDxResource
	{
	public:
		utils::ByteStream byteStream;
	};

}
