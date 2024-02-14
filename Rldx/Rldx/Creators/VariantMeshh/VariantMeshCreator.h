#pragma once

#include "..\..\ImportExport\Helpers\ByteStream.h"
#include "..\..\ImportExport\Libs\PugiXML\pugixml.hpp"
#include "..\..\SceneGraph\SceneNodes\DxVariantMeshhNode.h"

namespace rldx
{

	class VmdTagParser
	{
		pugi::xml_node* m_pXmlVmdNode;

	public:
		void ParseNode()
		{
			// TODO: ALGO for parsing XML tag to VmdNode
			/*
				tagname = current_xml.name()			
				NoderParserFactory.Get(tagName)

				
				//

			
			*/
			m_pXmlVmdNode->child("Slot")
		}

	private:
		void ParseSlots();
		void ParseVariantMeshes();
		void ParseMeshRefs();


	};

	class VMDCreator
	{
		pugi::xml_document m_xmlVmdDocument;

	public:
		rldx::DxVariantMeshhNode::SharedPtr Read(ByteStream& fileBytes)
		{

			m_xmlVmdDocument.load_buffer(fileBytes.GetBufferPtr(), fileBytes.GetBufferSize())


				// TODO:
				/*
					{ALGO 1; make tree with  asset info}
					- recurse through the XML
					- create 1 "VariantMeshNode" per tag
					- fil tag info into node (for later variant randomization (and saving)
					- for each tag that need parsing
					  - VARIANT_MESH_REFERENCE: request file from callback
					  - WSMODEL: parse, and store RMV2 and material INFO in node
					  - RMV2: do the same
						 - (maybe store WSMODEL/xmlmaterial, and RMV2 file material in the same structs)


					(- pause rendering, maybe)


				   {ALGO 2; load/reload assets in tree}
					- (if reloading, make disconnent root from scenegraph)
					- recurse through the VMD tree
					- load assets

					{"ALGO 3"; render}
					- when done, connect VMD tree root to scene graph



					(- restart rendering, if stopeed)

				*/



		}




	};
}