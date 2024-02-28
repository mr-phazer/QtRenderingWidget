#pragma once

#include "..\..\ImportExport\Helpers\ByteStream.h"
#include "IDxResource.h"

namespace rldx
{
	class DxResourceByteStream : public IDxResource
	{
	public:
		ByteStream byteStream;

		std::wstring GetTypeString() const { return L"ByteStream"; }
		ResourceTypeEnum GetType() const { return ResourceTypeEnum::BytesStream; }

	};

}
