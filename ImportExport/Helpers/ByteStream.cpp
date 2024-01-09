
#include "ByteStream.h"

// TODO: REMOVE
void DEBUG_func()
{
	const std::wstring file = LR"(some_file.rigid_model_v2)";
	auto diskReader = IByteStream::Create<DiskDataSource>(file);
	diskReader->
		
	auto memReader = IByteStream::Create<FixedMemoryByteStream>(pSomething, someSize);
	memReader->Read(&texture.type, 4);


}

void FixedMemoryByteStream::ReadData(void* pDest, size_t bytesTo)
{
}

void FixedMemoryByteStream::Read(void* pDest, size_t size, size_t startOffset)
{
}

uint8_t& FixedMemoryByteStream::operator[](size_t index)
{
	// TODO: insert return statement here
}
