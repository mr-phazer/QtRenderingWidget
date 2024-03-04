#pragma once

#include "..\..\..\Helpers\Templates.h"

// forward declartion to avoid too many #includes

namespace anim
{
	class AnimHeaderCommon;
}

class IAnimHeaderFileCommon : public  templates::TCreator <anim::AnimHeaderCommon> {};

class Anim_V6_HeaderFileCommon : public IAnimHeaderFileCommon
{
	virtual anim::AnimHeaderCommon Create(ByteStream& bytes) override;
};