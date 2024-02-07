#pragma once


class VariantMeshParser
{
	pugi::xml_document m_xmlDocument;

	VariantMeshParser(ByteStream input)
	{
		m_xmlDocument.load_buffer(input.GetBufferPtr(), input.GetBufferSize());
	}

	void Read(ID3D11Device* poDevice, DxBaseNode* rootNode, ByteStream& data)
	{
		/*
		algo:
			run through all children:


		*/
	}

	static bool IsTag_Attach_SLOT(const pugi::xml_node& xmlNode, wstring& name, wstring& attach_point)
	{
		auto Pred = [](wchar_t* sz) { return toUpper(sz) == VMDTag::AttachPoint; };
		xmlNode.find_attribute(Pred);

		auto Pred = [](wchar_t* sz) { return toUpper(sz) == VMDTag::Name; };
		xmlNode.find_attribute(Pred);



		bool isAttachSlot = false;
		auto attr = xmlNode.first_attribute();
		while (attr)
		{
			if (toUpper(attr.name()) == toUpper(VMDTag::AttachPoint))
			{



			}

			attr = attr.next_attribute();
		}

		return false;
	}

	void ParseTag(const pugi::xml_node& xmlNode)
	{
		if (IsTag(xmlNode, VMDTag::Slot))
			;

		if (IsTag(xmlNode, VMDTag::VariantMesh))
			;
		if (IsTag(xmlNode, VMDTag::VariantMeshRef))
			;


	}


};
