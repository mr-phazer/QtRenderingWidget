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
		TwAnimFile m_animFile;
		TwAnimFile* m_poBindPose = nullptr;

	public:
		explicit TwAnimFileReader(TwAnimFile* poBindPose = nullptr) : m_poBindPose(poBindPose) {};

		// TODO: Move skeleton param here
		// LIKE: TwAnimFile Read(ByteStream& bytes, TwAnimFile* poBindPose = nullptr);
		TwAnimFile Read(ByteStream& bytes);

	private:
		void ReadHeader(ByteStream& bytes);
		void ReadBoneTable(ByteStream& bytes);

		//---------------------------------------------------------------------//
		// Frame reading/processing
		//---------------------------------------------------------------------//
		void ReadSubClip(ByteStream& bytes);

		void ReadFrames_v7(ByteStream& bytes);

		AnimFrameCommon ReadConstTracksFrame_v7(ByteStream& bytes, const FrameHeaderCommon& constTrackFrameHeader);

		/// <summary>
		/// Reads compressed frame and decodes it into full frames
		/// </summary>		
		AnimFrameCommon ReadSingleFrame_v7(
			ByteStream& bytes,
			const BoneTrackMetaData_V7& metaTable,
			const FrameHeaderCommon&,
			const AnimFrameCommon* constTrackFrame = nullptr,
			const AnimFrameCommon* bindPoseFrame = nullptr
		);

		void ReadSingleFrame_v8(
			ByteStream& bytes,
			const BoneTrackMetaData_V8& metaTable,
			const FrameHeaderCommon&,
			const AnimFrameCommon* constTrackFrame = nullptr,
			const AnimFrameCommon* bindPoseFrame = nullptr
		);

		// TODO: REMOVE
		//void ReadFrameTranslations(
		//	const FrameHeaderCommon& frameHeader,
		//	const BoneTrackMetaData_V7& metaTable,
		//	AnimFrameCommon& outFrame,
		//	const AnimFrameCommon* constTrackFrame = nullptr,
		//	const AnimFrameCommon* bindPoseFrame = nullptr
		//);

		//void ReadFrameRotations(
		//	const FrameHeaderCommon& frameHeader,
		//	const BoneTrackMetaData_V7& metaTable,
		//	AnimFrameCommon& outFrame,
		//	const AnimFrameCommon* constTrackFrame,
		//	const AnimFrameCommon* bindPoseFrame);



		FrameHeaderCommon ReadConstTrackFrameHeader(ByteStream& bytes);
		FrameHeaderCommon ReadPartHeader(ByteStream& bytes);






		BoneTrackMetaData_V7 ReadTracksMetaTable_v7(ByteStream& bytes, uint32_t boneCount);
		BoneTrackMetaData_V8 ReadTracksMetaTable_v8(ByteStream& bytes, uint32_t boneCount);

	};
}