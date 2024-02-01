#pragma once

#include <fstream>
#include <vector>
#include <map>

#include <Windows.h>

#include "FileHelpers.h"


using ByteVector = std::vector<uint8_t>;

/// <summary>
/// Implementation of IByteStream that reads from memory
/// </summary>
class ByteStream
{
	ByteVector m_data;
	size_t m_currentOffset = 0;

	std::wstring m_currentFilePath = L"";

public:
	ByteStream(const ByteVector& input)
		: m_data(input) {};

	ByteStream(void* pMem, size_t sizeInBytes)
	{
		m_data.resize(sizeInBytes);
		memcpy(m_data.data(), pMem, sizeInBytes);
	}

	ByteStream(const std::wstring& fileName)
	{
		m_currentFilePath = fileName;
		m_data = file_helpers::ReadFileToVector(fileName);
	};

	ByteVector GetRawChunk(size_t bytes) const
	{
		if (m_currentOffset + bytes > m_data.size())
			throw std::exception("MemoryByteStream::GetRawChunk: out of bounds");

		return ByteVector(m_data.begin() + m_currentOffset, m_data.begin() + m_currentOffset + bytes);
	}

	ByteVector GetRawChunk(size_t bytes, long long offset)	const
	{
		if (offset + bytes >= m_data.size())
			throw std::exception("MemoryByteStream::GetRawChunk: out of bounds");

		return ByteVector(m_data.begin() + offset, m_data.begin() + offset + bytes);
	}	

	void Read(void* pDest, size_t bytesToCopy)
	{		
		if (m_currentOffset + bytesToCopy > m_data.size())
		{			
			throw std::exception("MemoryByteStream::Read: out of bounds");
		}

		memcpy(pDest, m_data.data() + m_currentOffset, bytesToCopy);
		m_currentOffset += bytesToCopy;
	}

	/// <summary>
	/// A template version of Read that will automatically determine the size of the type
	/// ...maybe
	/// </summary>	
	template <typename CONST_BUF_DATA_TYPE>
	void TAutoRead(CONST_BUF_DATA_TYPE* pDest)
	{
		auto bytesToCopy = sizeof(CONST_BUF_DATA_TYPE);

		if (m_currentOffset + bytesToCopy > m_data.size())
			throw std::exception("MemoryByteStream::TAutoRead: out of bounds");

		memcpy(pDest, m_data.data() + m_currentOffset, bytesToCopy);
		m_currentOffset += bytesToCopy;
	}

	template <typename CONST_BUF_DATA_TYPE>
	CONST_BUF_DATA_TYPE GetElement(std::size_t elementCount = 1)
	{
		CONST_BUF_DATA_TYPE element;
		Read(&element, sizeof(CONST_BUF_DATA_TYPE));
		return element;
	}	

	const uint8_t* GetBufferPtr() const { return m_data.data(); }	
	uint8_t* GetBufferPtr() { return m_data.data(); }
	
	size_t GetBufferSize() { return m_data.size(); }

	std::wstring GetPath() const { return m_currentFilePath; }

	bool IsValid() const
	{
		return m_data.size() > 0;
	};
};


// TODO: remove?
/// <summary>
/// Used to read a string of bytes
/// </summary>
//class IByteStream
//{
//public:
//	template<typename DERIVED_TYPE>
//	static std::unique_ptr<IByteStream> Create(const std::wstring& fileName)
//	{
//		return std::make_unique<DERIVED_TYPE>(fileName);
//	}
//
//	template<typename DERIVED_TYPE>
//	static std::unique_ptr<IByteStream> Create(void* pMem, size_t sizeInBytes)
//	{
//		return std::make_unique<DERIVED_TYPE>(pMem, sizeInBytes);
//	}
//
//	virtual void Read(void* pDest, size_t bytesToCopy, long long offset = -1) = 0;
//
//	virtual void Read(void* pDest, size_t bytesToCopy) = 0;
//
//	template <typename CONST_BUF_DATA_TYPE>
//	CONST_BUF_DATA_TYPE GetElement()
//	{
//		CONST_BUF_DATA_TYPE element;
//		Read(&element, sizeof(CONST_BUF_DATA_TYPE));
//		return element;
//	}
//};





// TODO: remove? Use?
// TODO: REMOVE?
//class CibstByteVector
//{
//	std::vector <uint8_t> m_data;
//
//public:
//	ConstVector(const std::vector<uint8_t>& input)
//		: m_data(input)	{};
//
//	ConstVector(size_t size, uint8_t value = 0)
//		: m_data(size, value)	{};
//
//	operator const std::vector<uint8_t>& () const
//	{
//		return m_data;
//	}
//
//	const std::vector<uint8_t>* operator->() const
//	{
//		return &m_data;
//	}
//
//	const std::vector<uint8_t>& operator*() const
//	{
//		return m_data;
//	}
//};
//
//void abd()
//{
//	ByteStream mbs(L"");
//	auto chunk = mbs.GetChunk(10);
//	chunk.Read(nullptr, 10);
//
//	ConstVector cv(10);
//	auto stuff = (*cv)[0];
//}
//

///// <summary>
///// Implementation of IByteStream that reads from a c++ (disk file) stream
///// </summary>
//class DiskDataSource : public IDataSource
//{
//	std::wstring m_filePath = L"";
//
//
//	DiskDataSource() = delete;
//	static std::unique_ptr<IDataSource> Create(const std::wstring& filePath)
//	{
//		auto newInstance = std::make_unique<DiskDataSource>();
//		newInstance->m_filePath = filePath;
//		newInstance->m_fileLength = GetFileSize(filePath);
//
//		return newInstance;
//	}
//
//
//	virtual void ReadAllData(ByteVector& dest) override
//	{
//		std::ifstream inputFileStream(m_filePath, std::ios::binary);
//		dest.resize(m_fileLength);
//		inputFileStream.read((char*)dest.data(), m_fileLength);
//		inputFileStream.close();
//	}
//};

//
///// <summary>
///// Used to read a string of binary from anywhere
///// </summary>
//class IDataSource
//{
//	long long m_dataLength = 0;
//
//	template<typename DERIVED_TYPE>
//	static std::unique_ptr<IDataSource> Create(const std::wstring& fileName)
//	{
//		return std::make_unique<DERIVED_TYPE>(fileName);
//	}
//
//	template<typename DERIVED_TYPE>
//	static std::unique_ptr<IDataSource> Create(void* pMem, size_t sizeInBytes)
//	{
//		return std::make_unique<DERIVED_TYPE>(pMem, sizeInBytes);
//	}
//
//	ByteVector GetAllData()
//	{
//		ByteVector buffer;
//		GetAllData(buffer);
//		return buffer;
//	}
//
//
//	virtual ByteVector GetAllData() = 0;
//};
