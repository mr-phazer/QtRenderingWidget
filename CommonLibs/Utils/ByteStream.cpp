#include "ByteStream.h"

#include <string>

#include "IOUtils.h"
#include "..\Logger\Logger.h"

namespace utils {
	using namespace logger;

	std::wstring ByteStream::sm_searchFolder = L"";

	ByteStream::ByteStream()
	{
		m_data.clear();
	}

	ByteStream::ByteStream(const ByteVector& input)
		: m_data(input) {}

	ByteStream::ByteStream(void* pMem, size_t sizeInBytes, const std::wstring fileName)
	{
		// TODO: is this the best error handling? m_data.size()==0 will make "IsValid()" return false, so maybe?
		if (sizeInBytes == 0) {
			m_data.clear();
			return;
		}

		if (pMem == nullptr) {
			m_data.clear();
			return;
		}

		m_data.resize(sizeInBytes);
		memcpy(m_data.data(), pMem, sizeInBytes);
		m_currentFilePath = fileName;
	}

	ByteStream::ByteStream(const std::wstring& fileName, bool doThrow)
	{
		if (DoesFileExist(fileName))
		{
			m_currentFilePath = fileName;
			ReadFileToVector(m_currentFilePath, m_data);
		}
		else if (DoesFileExist(sm_searchFolder + fileName))
		{
			m_currentFilePath = sm_searchFolder + fileName;
			ReadFileToVector(m_currentFilePath, m_data);
		}
		else
		{
			Logger::LogActionWarning(L"File: '" + fileName + L"', file does not exist.");

			if (doThrow)
				throw std::exception(ToString(L"ByteStream::ByteStream: '" + fileName + L"', file does not exist.").c_str());
			else
				m_data.clear();  // TODO: redundant?
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

	std::string ByteStream::ReadLengthPrefixed16StringA()
	{
		uint16_t stringLength_uint16 = 0;
		Read(&stringLength_uint16, sizeof(stringLength_uint16));

		std::string outString(stringLength_uint16, 0);

		Read((char*)outString.data(), stringLength_uint16);

		return outString;
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
}