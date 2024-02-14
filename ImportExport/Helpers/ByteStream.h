#pragma once

#include <vector>
#include <string>

using ByteVector = std::vector<uint8_t>;

/// <summary>
/// Implementation of IByteStream that reads from memory
/// </summary>
class ByteStream
{
	static std::wstring sm_searchFolder; // TODO: static for now, easiest, maybe change to fancier?

	std::wstring m_currentFilePath = L"";
	ByteVector m_data;
	size_t m_currentOffset = 0;	

public:
	ByteStream();
	ByteStream(const ByteVector& input);;
	ByteStream(void* pMem, size_t sizeInBytes, const std::wstring fileName = L"");
	ByteStream(const std::wstring& fileName, bool doThrow = true);;

	static void SetSearchFolder(const std::wstring& path);

	ByteVector GetAll() const;
	ByteVector GetRawChunk(size_t bytes) const;
	ByteVector GetRawChunk(size_t bytes, long long offset)	const;
	void Read(void* pDest, size_t bytesToCopy);

	// TODO: is the working?
	/// <summary>
	/// A template version of Read that will automatically determine the size of the type
	/// ...maybe
	/// </summary>	
	template <typename CONST_BUF_DATA_TYPE>
	void TAutoRead(CONST_BUF_DATA_TYPE* pDest);

	// TODO: is working?
	/// <summary>
	/// Get an "array" of a certain type, maybbe works
	/// </summary>	
	template <typename CONST_BUF_DATA_TYPE>
	CONST_BUF_DATA_TYPE GetElement(std::size_t elementCount = 1);

	const uint8_t* GetBufferPtr() const;	
	uint8_t* GetBufferPtr();

	size_t GetBufferSize() const;

	std::wstring GetPath() const;

	void SetOffset(size_t position);
	void Seek(int steps);

	bool IsValid() const;;
};

template<typename CONST_BUF_DATA_TYPE>
inline void ByteStream::TAutoRead(CONST_BUF_DATA_TYPE* pDest)
{
	auto bytesToCopy = sizeof(CONST_BUF_DATA_TYPE);

	if (m_currentOffset + bytesToCopy > m_data.size())
		throw std::exception("MemoryByteStream::TAutoRead: out of bounds");

	memcpy(pDest, m_data.data() + m_currentOffset, bytesToCopy);
	m_currentOffset += bytesToCopy;
}

template<typename CONST_BUF_DATA_TYPE>
inline CONST_BUF_DATA_TYPE ByteStream::GetElement(std::size_t elementCount)
{
	CONST_BUF_DATA_TYPE element;
	Read(&element, sizeof(CONST_BUF_DATA_TYPE));
	return element;
}
