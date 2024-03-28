#include <Helpers\ByteStream.h>
#include <Quantization\QuantTools.h>
#include "..\Common\AnimFrameCommon.h"
#include "QuantMetaTables.h"

namespace anim_file
{
	AnimTrackSourceEnum MetaElement_v7::GetTrackSourceState() const
	{
		if (m_metaValue >= 10000) return AnimTrackSourceEnum::ConstTrack;
		if (m_metaValue == -1) return AnimTrackSourceEnum::BindPose;
		return AnimTrackSourceEnum::FrameData;
	}

	uint32_t MetaElement_v7::GetConstTrackIndex() const
	{
		if (m_metaValue >= 10000) return m_metaValue - 10000;
		else
			throw std::exception("GetConstTrackIndex(): ERRRO: Meta element is not const track index!");
	}
};