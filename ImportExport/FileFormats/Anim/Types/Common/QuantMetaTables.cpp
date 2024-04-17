#include <Helpers\ByteStream.h>
#include <Quantization\QuantTools.h>
#include "..\Common\AnimFrameCommon.h"
#include "QuantMetaTables.h"

namespace anim_file
{
	BoneTrackDataSourceEnum_v5_v7 MetaElement_v7::GetTrackSourceState() const
	{
		if (m_metaValue >= 10000) return BoneTrackDataSourceEnum_v5_v7::ConstTrack;
		if (m_metaValue == -1) return BoneTrackDataSourceEnum_v5_v7::BindPose;
		return BoneTrackDataSourceEnum_v5_v7::FrameData;
	}

	uint32_t MetaElement_v7::GetConstTrackIndex() const
	{
		if (m_metaValue >= 10000) return m_metaValue - 10000;
		else
			throw std::exception("GetConstTrackIndex(): ERRRO: Meta element is not const track index!");
	}
};