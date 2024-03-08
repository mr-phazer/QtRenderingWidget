#pragma once

#include <SimpleMath.h>
#include "..\..\..\Helpers\ByteStream.h"
#include "..\Types\Common\AnimFrameCommon.h"
#include "..\Types\Common\FrameHeaders.h"
#include "..\Types\Common\TwAnimFile.h"
#include "..\Types\Raw\AnimFrameRaw.h"

namespace anim_file
{
	struct BoneTrackMetaData_V7;
	struct BoneTrackMetaData_V8;

	class TwAnimFileReader
	{
		// TODO: either make std::unique_ptr, or otherwise make sure it is cleared, if the reader instance it used more than once
		TwAnimFile m_animFile;
		TwAnimFile* m_poBindPose = nullptr;

	public:
		TwAnimFile Read(ByteStream& bytes, TwAnimFile* poBindPose = nullptr);

	private:
		void ReadHeader(ByteStream& bytes);

		// TODO: maybe do this (instead of the creator classes): 
		/*
		AnimCommonHeader ReadHeaderV6(ByteStream& bytes);
		AnimCommonHeader ReadHeaderV7(ByteStream& bytes);
		AnimCommonHeader ReadHeaderV8(ByteStream& bytes);
		*/

		void ReadBoneTable(ByteStream& bytes);
		void ReadSubClip(ByteStream& bytes);

		void ReadClip_v7(ByteStream& bytes);

		AnimFrameCommon ReadConstTracksFrame_v7(ByteStream& bytes, const FrameHeaderCommon& constTrackFrameHeader);
		AnimFrameCommon ReadConstTracksFrame_v8(ByteStream& bytes, const FrameHeaderCommon& constTrackFrameHeader);

		/// <summary>
		/// Decompressed frame into full frame data /
		/// </summary>		
		AnimFrameCommon DecodeFrame_V7(
			ByteStream& bytes,
			const BoneTrackMetaData_V7& metaTable,
			const FrameHeaderCommon&,
			const AnimFrameCommon* constTrackFrame = nullptr,
			const AnimFrameCommon* bindPoseFrame = nullptr
		);

		AnimFrameCommon DecodeFrame_V8(
			ByteStream& bytes,
			const BoneTrackMetaData_V8& metaTable,
			const FrameHeaderCommon&,
			const AnimFrameCommon* constTrackFrame = nullptr,
			const AnimFrameCommon* bindPoseFrame = nullptr
		);

		FrameHeaderCommon ReadConstTrackFrameHeader(ByteStream& bytes);
		FrameHeaderCommon ReadSubClipHeader(ByteStream& bytes);

		BoneTrackMetaData_V7 ReadTracksMetaTable_v7(ByteStream& bytes, uint32_t boneCount);
		BoneTrackMetaData_V8 ReadTracksMetaTable_v8(ByteStream& bytes, uint32_t boneCount);
	};
}