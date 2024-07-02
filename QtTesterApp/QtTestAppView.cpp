#include "QtTestAppView.h"

#include <QDirIterator>
#include <QFileInfo>
#include <qlayout.h>


#include "..\QtRenderingWidget\Constants\GameIdKeys.h"
#include "..\QtRenderingWidget\ExternFunctions\Creators.h"
#include "..\Rldx\Rldx\Helpers\DxMeshCreatorHelper.h"
#include "..\Rldx\Rldx\Managers\ResourceManager\DxResourceByteStream.h"
#include "Utils\ByteStream.h"

#include "..\Rldx\Rldx\Managers\ResourceManager\DxResourceManager.h"

// TODO: remove as many includes as possible
#include "..\ImportExport\FileFormats\Anim\Creators\AnimFileHeaderCreator.h"
#include "..\ImportExport\FileFormats\Anim\Reader\TwAnimReader.h"
#include "..\ImportExport\FileFormats\Anim\Reader\TwAnimReader.h"
#include "..\ImportExport\FileFormats\RigidModel\Readers\RigidModelReader.h"
#include "..\QtRenderingWidget\ExternFunctions\Creators.h"
#include "..\Rldx\Rldx\Helpers\DxMeshCreatorHelper.h"
#include "..\Rldx\Rldx\Managers\DxDeviceManager.h"
#include "..\Rldx\Rldx\Managers\ResourceManager\DxResourceByteStream.h"
#include "..\Rldx\Rldx\Managers\ResourceManager\DxResourceManager.h"
#include "TestData.h"

// TODO: REMOVE WHEN WORKING, make neater debugging stuff
#include <iostream>
#include "..\QtRenderingWidget\ExternFunctions\Creators.h" // TODO: change this, this is ugly, though I guess fine for testing.

QtMainWindowView::QtMainWindowView(QWidget* parent)
	: QMainWindow(parent)
{
	using namespace logging;
	Logger::LogActionSuccess(L"Starting the program.");
	setupUi(this);

	// TODO: maybe not have this constructor !!!! even for test program
	InitRenderView_DEBUG();

	setWindowTitle("TWViewer 0.0.1a - Total War Viewing/Conversion Tool");

	resize(1525, 1525);
}


QStringList DEBUG_GetAllFiles(const QString& path, const QString& extension)
{
	QFileInfoList list;
	QStringList listPaths;
	QDirIterator dirIt(path, QDirIterator::Subdirectories);
	while (dirIt.hasNext()) {
		dirIt.next();
		if (QFileInfo(dirIt.filePath()).isFile())
			if (QFileInfo(dirIt.filePath()).suffix() == extension)
			{
				list.append(QFileInfo(dirIt.filePath()));
				listPaths.push_back(QFileInfo(dirIt.filePath()).absoluteFilePath());

			}
	};

	return listPaths;
}

// TODO: REMOVE and cleanup
void QtMainWindowView::InitRenderView_DEBUG()
{
	// TODO: DOESN't WORK, get set to null, as the release version is called, make better debugging code, that doesn't affect the widget, maybe
	// Or really, the testerapp is not meant to run in Release anyway.
	auto instance = rldx::DxResourceManager::Instance(); // instantate "global" resource manager
	rldx::DxResourceManager::SetAssetFetchCallback(&DEBUG_Callback_FileGetter);

	auto ptestData = &test_app_data::testData_WH3_brt_grail_guardians_VMD;
	auto qAssetPath = QString::fromStdWString(ptestData->assetFolder);

	rldx::DxResourceManager::SetGameAssetFolder(qAssetPath.toStdWString());
	QString gameIdString = QString::fromStdWString(ptestData->gameId);

	QString globalAssetFolder = QString::fromStdWString(LR"(I:\Coding\Repos\QtRenderingWidget\Rldx\Rldx\RenderResources\)");
	SetAssetFolder(&globalAssetFolder);

	QString globalLogFolder = QString::fromStdWString(LR"(c:\temp\)");
	SetLogFolder(&globalLogFolder);

	auto renderWidget = CreateQRenderingWidget(this, &gameIdString, nullptr, nullptr);

	if (!renderWidget)
		return;

	ByteStream bytes(ptestData->filePath);
	QString fileName = QString::fromStdWString(bytes.GetPath().c_str());
	QByteArray qBytes((char*)bytes.GetBufferPtr(), bytes.GetBufferSize());
	QString outErrorString;


	PauseRendering(renderWidget);
	//AddNewPrimaryAsset(renderWidget, &fileName, &qBytes, &outErrorString);
	TESTCODE_AddNewPrimaryAsset(renderWidget, &fileName, &qBytes, &outErrorString);

	ResumeRendering(renderWidget);

	setCentralWidget(renderWidget);
}





QtMainWindowView::~QtMainWindowView()
{}
