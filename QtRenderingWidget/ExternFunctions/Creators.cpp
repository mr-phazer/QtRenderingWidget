#include "Creators.h"

#include <exception>

#include "..\..\Rldx\Rldx\Creators\DxSceneCreator.h"
#include "..\..\Rldx\Rldx\Managers\ResourceManager\DxResourceManager.h"

#include "..\QtObjects\Views\QtRenderView.h"

#include "Logger\Logger.h"

using namespace logging;

void SetLogPath(const QString& path)
{
	// TODO: implement
	//throw std::exception("The method or operation is not implemented.");
}

// TODO: clean up this, maybe throw it out and write a proper app
/// <summary>
/// Demostration/test of using callback to set resources
/// </summary>
static void DEBUG_Callback_FileGetter(QList<QString>* missingFiles, QList<QByteArray>* outBinFiles);

QWidget* CreateQRenderingWidget(
	QWidget* parent, 
	QString* gameIdString, 
	void (*AssetFetchCallBack) (QList<QString>* missingFiles, QList<QByteArray>* outBinFiles),
	void (*AnimPathsBySkeletonCallBack) (QString* skeletonName, QList<QString>* out)
)
{
	// Conditional compilation for debug vs release, debugging used my "simulated" callback, release uses the actual callback
#ifdef _DEBUG
	rldx::DxResourceManager::SetAssetFetchCallback(&DEBUG_Callback_FileGetter);
#else	
	rldx::DxResourceManager::SetAssetFetchCallback(AssetFetchCallBack);
	rldx::DxResourceManager::SetAnimPathsBySkeletonCallBack(AnimPathsBySkeletonCallBack);
#endif	

	QtRenderWidgetView* poNewRenderingWidget = nullptr;

	try {
		poNewRenderingWidget = new QtRenderWidgetView(parent, *gameIdString); // nullptr = no parent, free floating window			
		poNewRenderingWidget->hide();
		poNewRenderingWidget->InitRenderView();
		rldx::DxResourceManager::Instance()->SetGameIdSting(gameIdString->toStdWString()); // TODO: maybe ONLY store it here?
	}
	catch (std::exception& e)
	{
#ifdef _DEBUG
		MessageBoxA(reinterpret_cast<HWND>(parent->winId()), e.what(), "Error: Exception", MB_OK | MB_ICONERROR);
#endif

		LogAction(std::string("Error: Excpetion: ") + e.what());

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
		rldx::DxSceneCreator::AddModel(rldx::DxDeviceManager::Device(), currentScene, fileDataStream, gameIdString.toStdWString());
	}
	catch (std::exception& e)
	{
#ifdef _DEBUG
		MessageBoxA(reinterpret_cast<HWND>(pQRenderWiget->winId()), e.what(), "Error: Exception", MB_OK | MB_ICONERROR);
#endif

		* outErrorString = QString::fromStdString(std::string("Error: Excpetion: ") + e.what());
		LogAction(std::string("Error: Excpetion: ") + e.what());

		return false;
	}

	return true;
}

bool TESTCODE_AddNewPrimaryAsset(QWidget* pQRenderWiget, QString* assetFolder, QByteArray* assetData, QString* outErrorString)
{
	auto renderWidget = static_cast<QtRenderWidgetView*>(pQRenderWiget);
	auto gameIdString = renderWidget->GetGameIdString();
	auto currentScene = renderWidget->GetSceneManager()->GetCurrentScene();

	try {

		ByteStream fileDataStream(assetData->data(), assetData->size(), assetFolder->toStdWString());
		rldx::DxSceneCreator::AddVariantMesh(rldx::DxDeviceManager::Device(), currentScene, fileDataStream, gameIdString.toStdWString());
	}
	catch (std::exception& e)
	{
#ifdef _DEBUG
		MessageBoxA(reinterpret_cast<HWND>(pQRenderWiget->winId()), e.what(), "Error: Exception", MB_OK | MB_ICONERROR);
#endif
		* outErrorString = QString::fromStdString(std::string("Error: Excpetion: ") + e.what());

#ifdef _DEBUG
		logging::LogAction(std::string("Error: Excpetion: ") + e.what());
#endif
		return false;
	}

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


void DEBUG_Callback_FileGetter(QList<QString>* missingFiles, QList<QByteArray>* outBinFiles)
{

	outBinFiles->clear();
	for (size_t iAsset = 0; iAsset < missingFiles->size(); iAsset++)
	{

		if (IsDiskFile((*missingFiles)[iAsset].toStdWString())) {
			throw std::exception("Cannot accept disk files");
		}

		// TODO: the "GetGameAssetFolder" is only for "local callback" debuggin
		auto filePath = rldx::DxResourceManager::GetGameAssetFolder() + (*missingFiles)[iAsset].toStdWString();
		ByteStream newStream(filePath, false);

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
