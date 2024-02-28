#include "ByteStream.h"

#include <string>

#include "..\..\Rldx\Rldx\Logging\Logging.h"
#include "..\..\Rldx\Rldx\Tools\tools.h"
#include "FileHelpers.h"

std::wstring ByteStream::sm_searchFolder = L"";

ByteStream::ByteStream()
{
	m_data.clear();
}

ByteStream::ByteStream(const ByteVector& input)
	: m_data(input) {}

ByteStream::ByteStream(void* pMem, size_t sizeInBytes, const std::wstring fileName)
{
	m_data.resize(sizeInBytes);
	memcpy(m_data.data(), pMem, sizeInBytes);
	m_currentFilePath = fileName;
}

ByteStream::ByteStream(const std::wstring& fileName, bool doThrow)
{
	if (file_helpers::DoesFileExist(fileName))
	{
		m_currentFilePath = fileName;
		m_data = file_helpers::ReadFileToVector(m_currentFilePath);
	}
	else if (file_helpers::DoesFileExist(sm_searchFolder + fileName))
	{
		m_currentFilePath = sm_searchFolder + fileName;
		m_data = file_helpers::ReadFileToVector(m_currentFilePath);
	}
	else
	{
		logging::LogActionWarning("File: '" + libtools::wstring_to_string(fileName) + "', file does not exist.");

		if (doThrow)
			throw std::exception(std::string("ByteStream::ByteStream: '" + libtools::wstring_to_string(fileName) + "', file does not exist.").c_str());
		else
			m_data.clear();  // TODO: redundant
		return;
	}
}

void ByteStream::SetSearchFolder(const std::wstring& path) // TODO: static for now, easiest, maybe change to fancier?
{
	sm_searchFolder = path;
	if (sm_searchFolder[sm_searchFolder.size() - 1] != L'\\') // make sure search folder ends with slash to avoid incorent paths
	{
		sm_searchFolder += L'\\';
	}
}

ByteVector ByteStream::GetAll() const
{
	return m_data;
}

ByteVector ByteStream::GetRawChunk(size_t bytes) const
{
	if (m_currentOffset + bytes > m_data.size())
		throw std::exception("MemoryByteStream::GetRawChunk: out of bounds");

	return ByteVector(m_data.begin() + m_currentOffset, m_data.begin() + m_currentOffset + bytes);
}

ByteVector ByteStream::GetRawChunk(size_t bytes, long long offset) const
{
	if (offset + bytes >= m_data.size())
		throw std::exception("MemoryByteStream::GetRawChunk: out of bounds");

	return ByteVector(m_data.begin() + offset, m_data.begin() + offset + bytes);
}

void ByteStream::Read(void* pDest, size_t bytesToCopy)
{
	if (m_currentOffset + bytesToCopy > m_data.size())
	{
		throw std::exception("MemoryByteStream::Read: out of bounds");
	}

	memcpy(pDest, m_data.data() + m_currentOffset, bytesToCopy);
	m_currentOffset += bytesToCopy;
}

const uint8_t* ByteStream::GetBufferPtr() const { return m_data.data(); }

uint8_t* ByteStream::GetBufferPtr() { return m_data.data(); }

size_t ByteStream::GetBufferSize() const { return m_data.size(); }

std::wstring ByteStream::GetPath() const { return m_currentFilePath; }

void ByteStream::SetOffset(size_t position) {

	if (position >= m_data.size()) {
		throw std::exception((FULL_FUNC_INFO("ByteStream SetOffset out of bound >= data size")).c_str());
	}

	m_currentOffset = position;
}

void ByteStream::Seek(int steps)
{
	if (m_currentOffset + steps >= m_data.size() || m_currentOffset + steps < 0) {
		throw std::exception((FULL_FUNC_INFO("ByteStream Seek out of bounds > data size or < 0")).c_str());
	}

	m_currentOffset += steps;
}

bool ByteStream::IsValid() const
{
	return m_data.size() > 0;
}