#include "Creators.h"

#include <exception>
#include <CommonLibs\Logger\Logger.h>

#include <QtRenderingWidget\QtObjects\Views\QtRenderWidgetView.h>

using namespace logging;
using namespace utils;





//class CalllBackHelper
//{
//	void GetAssetsFromClient(QList<QString>* missingFiles, QList<QByteArray>* outBinFiles);
//	void GetAssetFromDisk(QList<QString>* missingFiles, QList<QByteArray>* outBinFiles);
//public:
//	void GetAssets(QList<QString>* missingFiles, QList<QByteArray>* outBinFiles)
//	{
//		AssetFetchCallBackFuncPtr* AssetFetchCallBackStored = nullptr; // TODO: maybe store it somewhere else? or pass it as parameter? or make this whole class static and store it in static variable?
//
//		if (AssetFetchCallBackStored != nullptr) {
//			GetAssetsFromClient(missingFiles, outBinFiles);
//		}
//		else {
//			GetAssetFromDisk(missingFiles, outBinFiles);
//		}
//	}
//};
//

QWidget* CreateQRenderingWidget(
	QWidget* parent,
	QString* gameIdString,
	CallBackFuncPtr assetFetchCallBack,
	void (*AnimPathsBySkeletonCallBack) (QString* skeletonName, QList<QString>* out)
)
{
	
	//setFetchCallBackStored = AssetFetchCallBack;
	  
	//szOID_RDN_TCG_PLATFORM_MODEL->

//	rldx::DxResourceManager::SetAssetFetchCallback(AssetFetchCallbackWrapper);
	//rldx::DxResourceManager::SetAnimPathsBySkeletonCallBack(AnimPathsBySkeletonCallBack);

	QtRenderWidgetView* poNewRenderingWidget = nullptr;

	try {
		poNewRenderingWidget = new QtRenderWidgetView(parent, *gameIdString, assetFetchCallBack); // nullptr = no parent, free floating window			
		poNewRenderingWidget->hide();

		poNewRenderingWidget->GetSceneManager()->GetResourceManager().SetGameIdSting(gameIdString->toStdWString()); // TODO: maybe ONLY store it here?		
	}
	catch (std::exception& e)
	{
#ifdef _DEBUG
		MessageBoxA(reinterpret_cast<HWND>(parent->winId()), e.what(), "Error: Exception", MB_OK | MB_ICONERROR);
#endif

		logging::LogAction(L"Error: Excpetion: " + utils::ToWString(e.what()));

		delete poNewRenderingWidget;
		return nullptr;
	}

	poNewRenderingWidget->setWindowFlag(Qt::WindowType::Widget);
	poNewRenderingWidget->setWindowTitle("QtRenderingWidget");// "QRenderingWidget (testing, with various stuff in layout, for testin.");
	poNewRenderingWidget->show();
	poNewRenderingWidget->StartRendering();

	

	return poNewRenderingWidget;
}

bool AddNewPrimaryAsset(QWidget* pQRenderWiget, QString* assetFolder, QByteArray* assetData, QString* outErrorString)
{
	auto renderWidget = static_cast<QtRenderWidgetView*>(pQRenderWiget);	
	renderWidget->GetController()->LoadNewAsset(renderWidget, assetFolder, assetData, outErrorString);

	renderWidget->PauseRendering();
	auto gameIdString = renderWidget->GetGameIdString();
	auto currentScene = renderWidget->GetSceneManager()->GetCurrentScene();
	auto& resoureceManager = renderWidget->GetSceneManager()->GetResourceManager();

	try {

		ByteStream fileDataStream(assetData->data(), assetData->size(), assetFolder->toStdWString());
		rldx::DxSceneCreator::AddVariantMesh(rldx::DxDeviceManager::Device(), resoureceManager, currentScene, fileDataStream, gameIdString.toStdWString());
	}
	catch (std::exception& e)
	{
#ifdef _DEBUG
		MessageBoxA(reinterpret_cast<HWND>(pQRenderWiget->winId()), e.what(), "Error: Exception", MB_OK | MB_ICONERROR);
#endif
		* outErrorString = QString::fromStdString(std::string("Error: Excpetion: ") + e.what());

#ifdef _DEBUG
		LogAction(L"Error: Excpetion: " + utils::ToWString(e.what()));
#endif
		return false;
	}

	renderWidget->ResumeRendering();

	return true;
}

void SetAssetFolder(QString* folder)
{
	ByteStream::SetSearchFolder(folder->toStdWString());
}

void SetLogFolder(QString* folder)
{
	Logger::SetLogFolder(folder->toStdWString());
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

// TODO: remove, when everythig above works
//bool OLD_AddNewPrimaryAsset(QWidget* pQRenderWiget, QString* assetFolder, QByteArray* assetData, QString* outErrorString)
//{
//	auto renderWidget = static_cast<QtRenderWidgetView*>(pQRenderWiget);
//	auto gameIdString = renderWidget->GetGameIdString();
//	auto currentScene = renderWidget->GetSceneManager()->GetCurrentScene();
//
//	try {
//
//		ByteStream fileDataStream(assetData->data(), assetData->size(), assetFolder->toStdWString());
//		rldx::DxSceneCreator::AddModel(rldx::DxDeviceManager::Device(), currentScene, fileDataStream, gameIdString.toStdWString());
//	}
//	catch (std::exception& e)
//	{
//#ifdef _DEBUG
//		MessageBoxA(reinterpret_cast<HWND>(pQRenderWiget->winId()), e.what(), "Error: Exception", MB_OK | MB_ICONERROR);
//#endif
//
//		* outErrorString = QString::fromStdString(std::string("Error: Excpetion: ") + e.what());
//		LogAction(L"Error: Excpetion: " + utils::ToWString(e.what()));
//
//		return false;
//	}
//
//	return true;
//}
