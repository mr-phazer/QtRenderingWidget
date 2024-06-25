#pragma once

// TODO: Forward declare anything possible
#include <SimpleMath.h>
#include "..\..\..\Helpers\ByteStream.h"
#include "..\Types\Common\AnimFrameCommon.h"
#include "..\Types\Common\FrameHeaders.h"
#include "..\Types\Common\TwAnimFile.h"

namespace anim_file
{
	struct CompressionMetaData_V5_V7;
	struct CompressionMetaData_V8;

	class TwAnimFileReader
	{
		// TODO: either make std::unique_ptr, or otherwise make sure it is cleared, if the reader instance it used more than once
		AnimFile m_animFile;
		const AnimFile* m_poBindPose = nullptr;

	public:
		AnimFile Read(ByteStream& bytes, const AnimFile* animFileBindPose = nullptr);

		std::wstring GetSkeletonName(ByteStream& bytes);
		// TODO: remove if above (Read()) works well
		//void SetBindPose(const AnimFile& animFileBindPose) { m_bindPose = animFileBindPose; }

		// for getting the version without reading the file
		static uint32_t GetAnimFileVersion(ByteStream& bytes);

	private:
		uint32_t ReadAnimFileVersion(ByteStream& bytes);

		AnimHeaderCommon ReadFileHeader(ByteStream& bytes);
		BoneTable ReadBoneTable(ByteStream& bytes);

		SubClipHeaderV8 ReadSubsClipHeader_v8(ByteStream& bytes);

		void ReadSubClip(ByteStream& bytes, const SubClipHeaderV8& subclipHeader);

		void ReadClip_v5(ByteStream& bytes);
		void ReadClip_v7(ByteStream& bytes);
		void ReadClip_v8(ByteStream& bytes);

		AnimFrameCommon ReadConstTracksFrame_v7(ByteStream& bytes, const FrameHeaderCommon& constTrackFrameHeader);
		AnimFrameCommon ReadConstTracksFrame_V8(ByteStream& bytes, const CompressionMetaData_V8& metaTable, const FrameHeaderCommon& frameHeader) const;

		/// <summary>
		/// Decompressed frame into full frame data /
		/// </summary>		
		AnimFrameCommon DecodeFrame_V7(
			ByteStream& bytes,
			const CompressionMetaData_V5_V7& metaTable,
			const FrameHeaderCommon&,
			const AnimFrameCommon* constTrackFrame = nullptr,
			const AnimFrameCommon* bindPoseFrame = nullptr
		);

		AnimFrameCommon DecodeFrame_V8(ByteStream& bytes, const CompressionMetaData_V8& metaTable) const;

		FrameHeaderCommon ReadConstTracksFrameHeader_v7(ByteStream& bytes);
		FrameHeaderCommon ReadSubClipFramesHeader(ByteStream& bytes);

		CompressionMetaData_V5_V7 ReadCompressionMetaData_v5_v7(ByteStream& bytes, uint32_t boneCount);
		CompressionMetaData_V8 ReadCompressionMetaData_v8(ByteStream& bytes, uint32_t boneCount);
	};

	class ITranslationReader
	{
	public:
		virtual sm::Vector3 ReadTranslation(ByteStream& bytes) = 0;
	};
}