#pragma once

#include <memory>

#include <QObject>

#include "..\Views\QtRenderView.h"

namespace rldx {
	class DxModelNode;
}

namespace rmv2 {
	class DxModelNode;
}

struct RenderDataModel
{
	// -- scenegraph node, contained asset being loaded
	std::shared_ptr<rldx::DxModelNode> m_currentModelNode;

	// -- asset parsed data
	rmv2::RigidModelFileCommon m_rmv2file;
	//  TODO: WSModelFile m_wsmodelFile;
	//  TODO: VariantMeshData m_vmdfile;
};

class QtRenderController  : public QObject
{
	Q_OBJECT

public:
	QtRenderController(QObject *parent);
	~QtRenderController();

	QWidget* CreateView(QWidget* parent, const QString& gameIdString);

	/// <summary>
	/// Ready an asset for the renderview	
	/// </summary>
	/// <param name="modelPath">internal/disk path to file</param>
	/// <param name="modelData">model asset binary data</param>		
	void ReadyModel(const QString& modelPath, const QByteArray& modelData)
	{
		/*
		TODO:
		algo: 
			parse modelData into "m_rmv2file"     (or "m_wsmodelFile" or "m_vmdfile")		
		*/
	}
	
	QVector<QPair<QString, QByteArray>> GetModelNeededResourceList() const;

	void SetModelResources(const QVector<QPair<QString, QByteArray>>& modelResources)
	{
		/*
		TODO:
		algo for RMV2			
			- LOOP: run through all "m_rmv2file" meshes:
				- Create meshdata from "m_rmv2file" mesh data
				- Create new material:
		            - search  "modelResources" for needed textures				
				- Add new mehdata to "m_currentModelNode"
		*/

		/*TODO: 
		algo for VMD
		  TODO: THINK 

		*/

	
	}

	void StartRendering()
	{
		/*
		TODO:
		algo:
			- add "m_currentModelNode" to scenegraph
			- start rendering
		*/
	}
	void PauseRendering();

private:
	QtRenderView* m_renderView;
	RenderDataModel m_model;
};




