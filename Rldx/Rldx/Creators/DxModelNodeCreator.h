#pragma once

#include "..\..\ImportExport\Helpers\Templates.h"
#include "..\..\ImportExport\RigidModel\Readers\RigidModelReader.h"
#include "..\SceneGraph\SceneNodes\DxModelNode.h"

#define PUGIXML_WCHAR_MODE
#include "..\..\ImportExport\Libs\PugiXML\pugixml.hpp"

class ID3D11Device;
class ByteStream;


namespace rldx {
	class DxScene;



	class IDxModelNodeCreator
	{
		virtual DxModelNode::SharedPtr CreateNode(ID3D11Device* poDevice, DxBaseNode* rootNode, ByteStream& data) = 0;
	};


	class ModelNodeCreator : public IDxModelNodeCreator
	{
		DxModelNode::SharedPtr CreateNode(ID3D11Device* poDevice, DxBaseNode* rootNode, ByteStream& fileData)  override
		{
			auto extention = libtools::GetFileExtension(fileData.GetPath());

			if (extention == L"rigid_model_v2")
			{
				return CreateRmv2Node(poDevice, fileData);
			}
			else if (extention == L"wsmodel")
			{

			}
			else if (extention == L"variantmeshdefinition")
			{

			}

			auto modelNodeRmv2 = rldx::DxNodeCreator::CreateNode<DxModelNode>("ModelNode 1");

			rmv2::RigidModelReader reader;
			auto rmv2Parsed = reader.Read(data);

		}


		DxModelNode::SharedPtr CreateNodeFromRmv2(ID3D11Device* poDevice, ByteStream& fileData /*, TODO: MaterialData*/)
		{
			rmv2::RigidModelReader rmv2Reader;
			auto rmv2File = rmv2Reader.Read(fileData);


			rmv2::RigidModelReader rigidModelFileReader;
			auto rmv2File = rigidModelFileReader.Read(fileData);

			auto modelNodeRmv2 = rldx::DxNodeCreator::CreateNode<DxModelNode>("ModelNode");
			modelNodeRmv2->SetMeshData(poDevice, rmv2File);
			modelNodeRmv2->LoadMaterialData(poDevice, rmv2File); // TODO add support for (parsed) WSMODEL			

			return modelNodeRmv2;
		}

		DxModelNode::SharedPtr CreateNodeFromWSMmodel(ID3D11Device* poDevice, ByteStream& fileData /*, TODO: MaterialData*/)
		{
			/*
				ALGO:
				parse WSMODEL xml
				store: model path
				material for each mesh [lod][part]		
			
			*/


		}
	};



	class ModelNodeCreatorFactory : public helpers::TFactory<class IDxModelNodeCreator, int>
	{

	};


	

}