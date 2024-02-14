#pragma once

#include <fstream>
#include <vector>
#include <map>

#include <Windows.h>

// TODO move thhese to .CPP
#include "FileHelpers.h"
#include "..\..\Rldx\Rldx\Tools\tools.h"
#include "..\..\Rldx\Rldx\Logging\Logging.h"


using ByteVector = std::vector<uint8_t>;

/// <summary>
/// Implementation of IByteStream that reads from memory
/// </summary>
class ByteStream
{
	static std::wstring sm_searchFolder; // TODO: static for now, easiest, maybe change to fancier?

	ByteVector m_data;
	size_t m_currentOffset = 0;

	std::wstring m_currentFilePath = L"";

public:
	ByteStream()
	{
		m_data.clear();
	}

	ByteStream(const ByteVector& input)
		: m_data(input) {};

	ByteStream(void* pMem, size_t sizeInBytes, const std::wstring fileName = L"")
	{
		m_data.resize(sizeInBytes);
		memcpy(m_data.data(), pMem, sizeInBytes);
		m_currentFilePath = fileName;
	}

	ByteStream(const std::wstring& fileName, bool doThrow = true)
	{
		if (file_helpers::DoesFileExist(fileName))
		{
			m_currentFilePath = fileName;
			m_data = file_helpers::ReadFileToVector(m_currentFilePath);
		}
		else if (file_helpers::DoesFileExist(sm_searchFolder + fileName))
		{
			m_currentFilePath = sm_searchFolder + fileName;
			m_data = file_helpers::ReadFileToVector(fileName);
		}
		else
		{
			if (doThrow)
				throw std::exception(std::string("ByteStream::ByteStream: '" + libtools::wstring_to_string(fileName) + "', file does not exist.").c_str());
			else
				m_data.clear();  // TODO: redundant
			return;
		}
	};

	static void SetSearchFolder(const std::wstring& path) // TODO: static for now, easiest, maybe change to fancier?
	{
		sm_searchFolder = path;
		if (sm_searchFolder[sm_searchFolder.size() - 1] != L'\\') // make sure search folder ends with slash to avoid incorent paths
		{
			sm_searchFolder += L'\\';
		}
	}

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

	void SetOffset(size_t position) {

		if (position >= m_data.size()) {
			throw std::exception((FULL_FUNC_INFO("ByteStream SetOffset out of bound >= data size")).c_str());
		}

		m_currentOffset = position;
	}

	void Seek(int steps)
	{
		if (m_currentOffset + steps >= m_data.size() || m_currentOffset + steps < 0) {
			throw std::exception((FULL_FUNC_INFO("ByteStream Seek out of bounds > data size or < 0")).c_str());
		}

		m_currentOffset += steps;
	}

	bool IsValid() const
	{
		return m_data.size() > 0;
	};
};
