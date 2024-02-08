#pragma once

#include "IDxResource.h"
#include "..\..\ImportExport\Helpers\ByteStream.h"

namespace rldx
{
	class DxResourceByteStream : public IDxResource
	{
	public:
		ByteStream byteStream;

		std::string GetTypeString() const { return "ByteStream"; }
		ResourceTypeEnum GetType() const { return ResourceTypeEnum::BytesStream;  }

	};

}
