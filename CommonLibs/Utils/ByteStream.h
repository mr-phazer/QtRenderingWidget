#pragma once

#include <string>
#include <vector>

namespace utils {

	using ByteVector = std::vector<uint8_t>;


	/// <summary>
	/// Implementation of IByteStream that reads from memory
	/// </summary>
	class ByteStream
	{
		// TODO: make a vector of search folders
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

		template <typename T>
		std::vector<T> GetChunk(size_t bytes);

		ByteVector GetAll() const;
		ByteVector GetRawChunk(size_t bytes) const;
		ByteVector GetRawChunk(size_t bytes, long long offset)	const;
		void Read(void* pDest, size_t bytesToCopy);

		std::string ReadLengthPrefixed16StringA();

		//iint32_t ReadInt32();


		// TODO: is the working?
		/// <summary>
		/// A template version of Read that will automatically determine the size of the type
		/// ...maybe
		/// </summary>
		template <typename T>
		void TRead(T* pDest);

		// TODO: is working?
		/// <summary>
		/// Get an "array" of a certain type, maybbe works
		/// </summary>
		template <typename DATA_TYPE>
		DATA_TYPE TReadElement();

		template <typename T>
		std::vector<T> GetElements(std::size_t elementCount = 1);

		const uint8_t* GetBufferPtr() const;
		uint8_t* GetBufferPtr();

		size_t GetBufferSize() const;

		std::wstring GetPath() const;

		void SeekAbsolute(size_t position);
		void SeekRelative(int steps);

		bool IsValid() const;;
	};
}