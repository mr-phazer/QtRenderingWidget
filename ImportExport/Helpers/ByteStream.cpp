
#include "ByteStream.h"

// TODO: REMOVE
void DEBUG_func()
{
	const std::wstring file = LR"(some_file.rigid_model_v2)";
	auto diskReader = IByteStream::Create<DiskDataSource>(file);
	diskReader->Read(&texture.type, 4);
		
	auto memReader = IByteStream::Create<MemoryDataSource>(pSomething, someSize);
	memReader->Read(&texture.type, 4);


}