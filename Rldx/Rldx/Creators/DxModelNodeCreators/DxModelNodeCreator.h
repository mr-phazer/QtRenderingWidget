#pragma once

#include "..\..\ImportExport\Helpers\Templates.h"

#include "..\..\SceneGraph\SceneNodes\DxModelNode.h"
#include "..\..\..\Rldx\Tools\tools.h"

#include "DxRmv2ModelNodeCreator.h"
#include "DxWSModelModelnodeCreator.h"


namespace rldx {
	class DxScene;


	class ModelLoaderByExtensionFactory : public templates::TFactoryWStringKey<IDxModelNodeCreator>
	{	
		
	public:
		ModelLoaderByExtensionFactory()
		{
			Register<DxRmv2ModelNodeCreator>(L"rigid_model_v2");
			Register<DxWSModelModelnodeCretor>(L"wsmodel");
		//	Register<VMDNodeCreator>(L"variantmesh");		
		}
		
		void HandleKeyNotFound(std::wstring key)
		{
			throw std::exception(("Invalid File Type: " + libtools::wstring_to_string(key)).c_str());
		}
		
	};

	class ModelNodeCreator : public IDxModelNodeCreator
	{
		ModelLoaderByExtensionFactory m_moderNodeCreatorByExtension;

	public:
		DxModelNode::SharedPtr CreateNode(ID3D11Device* poDevice, DxBaseNode* rootNode, ByteStream& fileData)  override
		{
			auto extentsion = libtools::GetFileExtension(fileData.GetPath());

			auto modeleNodeCreator = m_moderNodeCreatorByExtension.Get(extentsion);

			auto modelNode = modeleNodeCreator->CreateNode(poDevice, rootNode, fileData);
			
			return modelNode;
		}
	};


};
	

