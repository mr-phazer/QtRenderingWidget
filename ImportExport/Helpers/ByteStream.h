#pragma once

#include <fstream>
#include <vector>

#include "FileHelpers.h"

using ByteVector = std::vector<uint8_t>;


/// <summary>
/// Used to read a string of bytes
/// </summary>
class IByteStream
{
public:
	template<typename DERIVED_TYPE>
	static std::unique_ptr<IByteStream> Create(const std::wstring& fileName)
	{
		return std::make_unique<DERIVED_TYPE>(fileName);
	}

	template<typename DERIVED_TYPE>
	static std::unique_ptr<IByteStream> Create(void* pMem, size_t sizeInBytes)
	{
		return std::make_unique<DERIVED_TYPE>(pMem, sizeInBytes);
	}
		
	virtual void ReadStreamData(void* pDest, size_t bytesTo) = 0;
	virtual void ReadStreatData(void* pDest, size_t size, size_t startOffset = 0) = 0;

	virtual uint8_t& operator[](size_t index) = 0;

protected:
};


/// <summary>
/// Used to read a string of binary from anywhere
/// </summary>
class IDataSource
{
	template<typename DERIVED_TYPE>
	static std::unique_ptr<IDataSource> Create(const std::wstring& fileName)
	{
		return std::make_unique<DERIVED_TYPE>(fileName);
	}

	template<typename DERIVED_TYPE>
	static std::unique_ptr<IDataSource> Create(void* pMem, size_t sizeInBytes)
	{
		return std::make_unique<DERIVED_TYPE>(pMem, sizeInBytes);
	}
	
	virtual ByteVector ReadAllData() = 0;
};

/// <summary>
/// Implementation of IByteStream that reads from a c++ (disk file) stream
/// </summary>
class DiskDataSource : public IDataSource
{
	std::wstring m_filePath = L"";
	long long m_fileLength = 0;

	DiskDataSource() = delete;
	static std::unique_ptr<IDataSource> Create(const std::wstring& filePath)
	{
		auto newInstance = std::make_unique<DiskDataSource>();
		newInstance->m_filePath = filePath;
		newInstance->m_fileLength = file_helpers::GetFileSize(filePath);

		return newInstance;
	}

	virtual ByteVector ReadAllData() override
	{			
		std::ifstream inputFileStream(m_filePath, std::ios::binary);		
		ByteVector buffer(m_fileLength);
		inputFileStream.read((char*)buffer.data(), m_fileLength);
		inputFileStream.close();

		return buffer;
	}		
};

/// <summary>
/// Implementation of IByteStream that reads from memory
/// </summary>
class MemoryByteStream : public IByteStream
{
	ByteVector m_data;
	size_t m_currentOffset = 0;

public:
	MemoryByteStream(const ByteVector& input)
		: m_data(input)
	{
	};


	// Inherited via IByteStream
	void Read(void* pDest, size_t bytesToCopy) override;
	void Read(void* pDest, size_t size, size_t startOffset) override;

	uint8_t& operator[](size_t index) override;

};