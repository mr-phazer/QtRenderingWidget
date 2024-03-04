#pragma once

#include <DirectXMath.h>

struct AttachmentPointTableEntry_V6_V7_V8
{
	static size_t constexpr GetSize() { return 84; }; // (32) + (3 * 4 * 4) + 4;

	// name of attachment point bone, has to be a bone from the skeleton referred to in the file header
	char szAttachmentPointName[32];	

	// transformtion, scale, rotation, translation are used by the game, but vanilla model always (?) have this to indenity
	DirectX::XMFLOAT3X4 o3x4Matrix =DirectX::XMFLOAT3X4(1, 0, 0, 0,	0, 1, 0, 0,	0, 0, 1, 0);

	uint32_t dwBoneId = 0; // attachment point bne id, referes to a bone in the skeleton that something (props) can be attached to.
};
