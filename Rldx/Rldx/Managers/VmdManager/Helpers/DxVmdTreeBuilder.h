#pragma once

#include "..\..\..\..\..\ImportExport\Helpers\Templates.h"
#include "..\..\..\SceneGraph\SceneNodes\VariannMeshNode\DxVmdNode.h"
#include "VmdNodeCreators.h"

namespace rldx
{

	class VmdNodeTreeBuilder
	{
		std::vector<std::wstring> m_attachPoints;

		class Fact : public templates::TFactoryStringKey<IDxVmdNodeAllocator> 
		{
			// Inherited via TFactoryStringKey
			void HandleKeyNotFound(std::string KeyValue) override
			{
				throw LogException("Stuff");
			}
		} factory;

	public:
		void Build(DxVmdNode* sceneGraphRoot, const pugi::xml_node& xmlNode);


	};

}