#pragma once

#include "..\..\..\Helpers\Templates.h"
#include "..\Types\Common\FileHeader.h"

class IAnimHeaderCommonCreator : public  templates::TCreator <anim_file::AnimHeaderCommon> {};

class Anim_V6_HeaderFileCommonCreator : public IAnimHeaderCommonCreator
{
public:
	virtual anim_file::AnimHeaderCommon Create(ByteStream& bytes) override;
};

class Anim_V7_HeaderFileCommonCreator : public IAnimHeaderCommonCreator
{
public:
	virtual anim_file::AnimHeaderCommon Create(ByteStream& bytes) override;
};

