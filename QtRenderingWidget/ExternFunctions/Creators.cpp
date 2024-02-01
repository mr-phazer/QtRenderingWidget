#include "Creators.h"

#include <exception>

#include "..\..\Rldx\Rldx\Managers\ResourceManager\DxResourceManager.h"
#include "..\..\Rldx\Rldx\Helpers\DxSceneCreator.h"

#include "QtObjects\Views\QtRenderView.h"

#include "..\rldx\rldx\Logging\Logging.h"

void SetLogPath(const QString& path)
{
	// TODO: implement
	//throw std::exception("The method or operation is not implemented.");
}

void SetAssetPath(const QString& path)
{
	// TODO: implement
	//throw std::exception("The method or operation is not implemented.");
}

/// <summary>
/// Demostration/test of using callback to set resources
/// </summary>
static void FileGetter(QList<QString>* missingFiles, QList<QByteArray>* outBinFiles)
{	
	outBinFiles->clear();	
	for (size_t iAsset = 0; iAsset < missingFiles->size(); iAsset++)
	{
		auto fileName = (*missingFiles)[iAsset].toStdWString();
		ByteStream newStream(fileName);
		
		// force list to be same size, maybe redundant
		*outBinFiles = QList<QByteArray>::fromVector(QVector<QByteArray>(missingFiles->size()));

		if (!newStream.IsValid())
		{
			(*outBinFiles)[iAsset] = QByteArray();
			continue;
		}
		
		// -- resize dest, buffer, and read whole file into it
		(*outBinFiles)[iAsset].resize(newStream.GetBufferSize());
		newStream.Read((*outBinFiles)[iAsset].data(), (*outBinFiles)[iAsset].size());		
	}
}

QWidget* CreateQRenderingWidget(QWidget* parent, QString* gameIdString, void (*AssetFetchCallBack) (QList<QString>* missingFiles, QList<QByteArray>* outBinFiles))
{
#ifdef _DEBUG
	rldx::DxResourceManager::SetcAssetFetchCallback(&FileGetter);
#else
	rldx::DxResourceManager::SetcAssetFetchCallback(AssetFetchCallBack);
#endif	
	
	QtRenderView* poNewRenderingWidget = nullptr;

	try {
		poNewRenderingWidget = new QtRenderView(parent); // nullptr = no parent, free floating window	
		poNewRenderingWidget->hide();
		poNewRenderingWidget->Init();

	}
	catch (std::exception& e)
	{
		MessageBoxA(reinterpret_cast<HWND>(parent->winId()), e.what(), "Error: Exception", MB_OK | MB_ICONERROR);
		logging::LogAction(std::string("Error: Excpetion: ") + e.what());

		delete poNewRenderingWidget;
		return nullptr;
	}

	poNewRenderingWidget->setWindowFlag(Qt::WindowType::Widget);
	poNewRenderingWidget->setWindowTitle("BLAH");// "QRenderingWidget (testing, with various stuff in layout, for testin.");
	poNewRenderingWidget->show();
	poNewRenderingWidget->StartRendering(/*&spManager*/);

	return poNewRenderingWidget;
}

void AddNewPimaryAsset(QWidget* pQRenderWidget, QString* assetPath, QByteArray* assetData)
{
	auto renderWidget = static_cast<QtRenderView*>(pQRenderWidget);

	auto currentScene = renderWidget->GetSceneManager().GetCurrentScene();

	ByteStream fileDataStream(assetData->data(), assetData->size());
	rldx::DxNativeWindowSceneBuilder::AddModel(rldx::DxDeviceManager::Device(), currentScene, fileDataStream);
}
