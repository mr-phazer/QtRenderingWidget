#include "Creators.h"

#include <exception>

#include "..\..\Rldx\Rldx\Managers\ResourceManager\DxResourceManager.h"
#include "..\..\Rldx\Rldx\Creators\DxSceneCreator.h"

#include "QtObjects\Views\QtRenderView.h"

#include "..\rldx\rldx\Logging\Logging.h"

void SetLogPath(const QString& path)
{
	// TODO: implement
	//throw std::exception("The method or operation is not implemented.");
}

/// <summary>
/// Demostration/test of using callback to set resources
/// </summary>
static void DEBUG_Callback_FileGetter(QList<QString>* missingFiles, QList<QByteArray>* outBinFiles);

QWidget* CreateQRenderingWidget(QWidget* parent, QString* gameIdString, void (*AssetFetchCallBack) (QList<QString>* missingFiles, QList<QByteArray>* outBinFiles))
{
// Conditional compilation for debug vs release, debugging used my "simulated" callback, release uses the actual callback
#ifdef _DEBUG
	rldx::DxResourceManager::SetAssetFetchCallback(&DEBUG_Callback_FileGetter);
#else
	rldx::DxResourceManager::SetAssetFetchCallback(AssetFetchCallBack);
#endif	
	
	QtRenderWidgetView* poNewRenderingWidget = nullptr;

	try {
		poNewRenderingWidget = new QtRenderWidgetView(parent); // nullptr = no parent, free floating window	
		poNewRenderingWidget->hide();
		poNewRenderingWidget->InitRenderView();		
		poNewRenderingWidget->SetGameIdString(*gameIdString);
	}
	catch (std::exception& e)
	{
#ifdef _DEBUG
		MessageBoxA(reinterpret_cast<HWND>(parent->winId()), e.what(), "Error: Exception", MB_OK | MB_ICONERROR);
#endif
		
		logging::LogAction(std::string("Error: Excpetion: ") + e.what());

		delete poNewRenderingWidget;
		return nullptr;
	}

	poNewRenderingWidget->setWindowFlag(Qt::WindowType::Widget);
	poNewRenderingWidget->setWindowTitle("BLAH");// "QRenderingWidget (testing, with various stuff in layout, for testin.");
	poNewRenderingWidget->show();
	poNewRenderingWidget->StartRendering();
	

	return poNewRenderingWidget;
}

bool AddNewPrimaryAsset(QWidget* pQRenderWiget, QString* assetFolder, QByteArray* assetData, QString* outErrorString)
{
	auto renderWidget = static_cast<QtRenderWidgetView*>(pQRenderWiget);
	auto gameIdString = renderWidget->GetGameIdString();
	auto currentScene = renderWidget->GetSceneManager()->GetCurrentScene();

	try {

		ByteStream fileDataStream(assetData->data(), assetData->size(), assetFolder->toStdWString());
		rldx::DxSceneCreator::AddModel(rldx::DxDeviceManager::Device(), currentScene, fileDataStream, gameIdString.toStdString());
	}
	catch (std::exception& e)
	{
#ifdef _DEBUG
		MessageBoxA(reinterpret_cast<HWND>(pQRenderWiget->winId()), e.what(), "Error: Exception", MB_OK | MB_ICONERROR);
#endif

		*outErrorString = QString::fromStdString(std::string("Error: Excpetion: ") + e.what());
		logging::LogAction(std::string("Error: Excpetion: ") + e.what());				

		return false;
	}

	return true;
}

void SetAssetFolder(QString* folder)
{
	rldx::DxResourceManager::SetAssetFolder(folder->toStdWString());
}

void PauseRendering(QWidget* pQRenderWiget)
{
	auto renderWidget = static_cast<QtRenderWidgetView*>(pQRenderWiget);
	renderWidget->PauseRendering();
}

void ResumeRendering(QWidget* pQRenderWiget)
{
	auto renderWidget = static_cast<QtRenderWidgetView*>(pQRenderWiget);
	renderWidget->ResumeRendering();
}

void DEBUG_Callback_FileGetter(QList<QString>* missingFiles, QList<QByteArray>* outBinFiles)
{
	outBinFiles->clear();
	for (size_t iAsset = 0; iAsset < missingFiles->size(); iAsset++)
	{
		auto fileName = rldx::DxResourceManager::GetAssetFolder() + (*missingFiles)[iAsset].toStdWString();
		ByteStream newStream(fileName, false);

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
