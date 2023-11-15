#pragma once

//#include "..\..\pch.h"


// ---- d3d 11 includes -----
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXColors.h>

#include <DirectXMath.h>
#include "..\..\..\DirectXTK\Inc\SimpleMath.h"

//#include <DirectXCollision.h>

#include <string>
#include <vector>

//// ---- author includes -----
////#include "..\..\Scene.h"
//#include "..\..\RenderLib\dx_const_buffers.h"
//#include "..\..\render_constants.h"
//#include "..\..\Shader.h"
//#include "..\..\IAbstractNode.h"
//#include "..\..\DXMeshBuffer.h"
//#include "..\..\SystemLib\tree_node.h"
//#include "..\..\RenderQueue.h"

#include "..\NodeTransform\NodeTransform.h"


#include <memory>  




namespace rendutil
{
	/// <summary>
	/// The Basic Node Of The ScenGraph
	/// </summary>
	class BaseNode;
	class BaseNode : //public IAbstractRenderObject,
		// inherited from to enable "this" smart pointers
		public std::enable_shared_from_this<BaseNode>
	{
		static bool sm_benableCubes;


		friend class Scene;
		
	public:
		using SPBaseNode = std::shared_ptr<BaseNode>;
		enum LocalRoot : bool
		{
			LocalRootNode = true,
			Default = false
		};

	public:
		// global flags
		static size_t sm_LOD_to_render;
		static bool sm_bShowNormals;
		static bool sm_bShowTangents;
		static bool sm_bShowBitangents;
		static float sm_fNormalLength;	
		

	
	public:
		BaseNode();
		virtual ~BaseNode()
		{
#ifdef _DEBUG
			//_log_action("Destroyed: " + m_strName);
#endif

			this->DeleteNode();
		};

		virtual bool init(ID3D11Device* _pDXDevice);

	public:
		const std::string& getName();
		void setName(const string& _strName);

		static SPBaseNode create(ID3D11Device* _poDXDevice, const std::string& _strName, NodeTypeEnum NodeType = NodeTypeEnum::eBaseNode);
		static SPBaseNode create(ID3D11Device* _poDXDevice, const std::string& _strName, bool a_bIsRootNode);

		virtual bool makeCubeMesh(ID3D11Device*);
		virtual bool updateBoundingBoxMesh(ID3D11Device*);

		
		

		bool areChildrenVisible() const;

		//void setDisabled(bool _state = true) { m_bEn;
		//virtual void setDraw(bool _bState = true);

		virtual void setChildrenVisible(bool _bState = true);		
		virtual void setNodeType(NodeTypeEnum _NodeType);


		void setEnabled(bool _state = true) {
			m_bEnabled = _state;
		};

		void setDraw(bool _state) {
			m_bDraw = _state;
		};

		void setBBVisible(bool _state) {
			m_bBoundingBoxVisible = _state;
		};

		bool isEnabled() const { return m_bEnabled; };
		bool isBBVisible() const { return m_bBoundingBoxVisible; };
		bool isDrawn() const { return m_bDraw; };
			
		
			
		/*****************************************
			Tree Management
		*******************************************/
		BaseNode* getRoot();

		void AddChild(SPBaseNode _poBaseNode);
		void AppendChild(SPBaseNode _poBaseNode);
		void InsertChild(size_t index, SPBaseNode);
		void AppendChildren(std::vector<SPBaseNode>& children);
		void EraseChildren();
		void DeleteChildren(bool bRecursive = true);
		bool RemoveFromParent();
		bool DeleteNode(); // delete node and all children
		static bool DeleteNode(SPBaseNode& _spoNode); // finds node, and deletes it
		SPBaseNode getParent();

		size_t getChildCount();
		SPBaseNode findFirstChild(const std::string& _strName);
		BaseNode* findNodeRecursive(const std::string& _strName);
		SPBaseNode getChildByIndex(size_t _index);
		
		std::vector<SPBaseNode>& getChildren() {
			return m_vecChildren;
		};

		virtual void clearChildren();

		/*
			Node debugging graphics indications
		*/
		bool setCubeVisible(bool _bState = true);

		operator bool(); // makes it possible use "BaseNode objects in boolean expressions
				
		/// <summary>
		/// Should be called from root to update all global node transforms
		/// </summary>
		void updateGlobalMatricesRecursive();
		
		void virtual updateGlobalTransform()
		{
			if (getParent())
			{
				nodeTransform.doGlobalTransform(getParent()->nodeTransform.getGlobalTransformMatrix());


				
			} else
			{
				nodeTransform.getGlobalTransformMatrix() = nodeTransform.getLocalTransform();
			}
		}

		// contains the node transform
		NodeTransform nodeTransform;

	private:
		// using "weeak_ptr" is it does not increase the ref count
		// so the object "parent" points can be deleted, even if nodes have it as parent
		std::weak_ptr<BaseNode> m_poParent;
		std::weak_ptr<BaseNode> poRoot;

		std::vector<std::SPBaseNode> m_vecChildren;
		static std::vector<std::SPNode> sm_vecTestChildren;


		bool _allocateCube(rendutil::Scene* _poScene);
		void makeLines(rendutil::Scene* _poScene, std::vector<DirectX::XMFLOAT4>& vertices, std::vector<uint16_t>& indices);
		void makeLineRecursvice(rendutil::
			Scene* _poScene, std::vector<DirectX::XMFLOAT4>& vertices, std::vector<uint16_t>& indices);

		std::vector<DirectX::XMFLOAT4> m_vertices;
		std::vector<uint16_t> m_indices;

	public:
		void setNodeShaderType(EShaderState type) {	 attributes.shaderState = type; };
		
		EShaderState getNodeShaderType(EShaderState type) const { return attributes.shaderState;	};
		
		void setNodeId(int32_t _dwId) { m_dwNodeId = _dwId; };
		
		int32_t getNodeId() const { return m_dwNodeId; };		
		
		int32_t getSkeletonId() const { return m_dwDeformerSkeletonId; };
		
		Skeleton* getDeformerSkeleton() const { return m_poDeformerSkeleton; };

		// set the skeleton this node's meshes are deformed by
		void setDeformerSkeleton(Skeleton* _spoSkeleton);

		

		//avoid setMeshesInheritsShaderType();
		
		virtual bool getMeshBuffersToDraw(DXMeshBufferstd::vector& _meshes);
			



	   // get internal mesh buffer for node, like "node-cubes", bounding boxes?
	   virtual bool getActiveMeshes_Private(DXMeshBufferstd::vector& _meshes);


		BaseNode* searchRecursiveForString(BaseNode* _popArent, const string& _strName);

		BaseNode* searchRecursiveForNode(const SPBaseNode& _spoNode);
		BaseNode* deleteNodeRescusive(BaseNode* _popArent, const SPBaseNode& _spoNode);

	
		void setIsValid(bool _state) {
			m_bIsValid = _state;
		};

		bool getIsValid() const {
			return m_bIsValid;
		};

		bool IsRootNode() const {
			return m_bIsRootNode;
		};


	

	/*	template<typename TYPE>
		TYPE* getAs()
		{
			return dynamic_cast<TYPE*>(this);
		}*/

		void updatestd::vectors(ID3D11Device* _pDC, ID3D11DeviceContext* _pDC1, size_t lod, size_t mesh);


protected:
		bool m_bMeshEnheritsShaderType = false;		
		Skeleton* m_poDeformerSkeleton = nullptr;
		uint32_t m_dwDeformerSkeletonId = 0;
		uint32_t m_dwNodeId = 1;	

		Shader::sptrShader spoShader = nullptr;	
		
		//DirectX::BoundingBox BoundBoxAA;
		std::vector<BoundingBox*> m_currentBoundingBoxes; // all the bounding boxes from the meshhes from the this node

		spDXMeshBuffer  m_oBoundBox_Lines;
		spDXMeshBuffer m_spoSceneGraphNodeCube;

		bool m_bLocalTransformDirty = true; // recalculate transform when "true"
		// axis aligned bounding box points
		sm::std::vector3 m_vAABB[2] = { {0,0,0}, {0,0,0} };


private:
		bool m_bEnabled = true; // draw the meshes of this node?
		bool m_bDraw = true; // draw the meshes of this node?
		bool m_bChildrenVisible = true; // draw the meshes of the nodes's children? (false = all ancenstors are also ignored)
		
		

		//bool m_bInheritStates = true; // if false, the children are not processed		
		//bool m_bDrawNode_Is_the_the_same_as_childreVisible = true;

		bool m_bIsValid = false;
		bool m_bIsCubeVisible = false;
		bool m_bBoundingBoxVisible = false;

		bool m_bIsRootNode = false;

		static uint32_t sm_next_id;

	public:
		static std::vector<DirectX::XMFLOAT3>  sm_vecCubeVertices;
		static std::vector<unsigned short int> sm_vecCubeIndices;

		//// the LOD sorted mesh buffer for the node,
		//IDXmeshContainerMultple oMesh_LODS; 			
		

//		RigidModelV2::Common::RMV2CommonFile::sptrRigidModel m_spoRMV2File;
	

	};
};


namespace rnd = rendutil;
