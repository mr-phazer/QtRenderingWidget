#pragma once

#include "Utils\ByteStream.h"
#include "IDxResource.h"

namespace rldx
{
	class DxResourceByteStream : public IDxResource
	{
	public:
		utils::ByteStream byteStream;

		std::wstring GetTypeString() const { return L"ByteStream"; }
		ResourceTypeEnum GetType() const { return ResourceTypeEnum::BytesStream; }

	};

}
