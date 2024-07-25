#include "QtTestAppView.h"

// Qt headers
#include <QDirIterator>
#include <qdockwidget.h>
#include <QFileInfo>
#include <qlayout.h>
#include <QMimeData>

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
#include <qcoreapplication.h>
#include "..\QtRenderingWidget\ExternFunctions\Creators.h" // TODO: change this, this is ugly, though I guess fine for testing.

using namespace utils;

QtMainWindowView::QtMainWindowView(QWidget* parent)
	: QMainWindow(parent)
{
	logging::LogAction(L"Starting the program.");
	setupUi(this);

	setAcceptDrops(true);

	// TODO: maybe not have this constructor !!!! even for test program
	DEBUG_InitRenderView();

	setWindowTitle("TWViewer 0.0.1a - Total War Viewing/Conversion Tool");

	resize(1525, 1525);
}


QtMainWindowView::~QtMainWindowView()
{}

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
void QtMainWindowView::DEBUG_InitRenderView()
{
	// TODO: DOESN't WORK, get set to null, as the release version is called, make better debugging code, that doesn't affect the widget, maybe
	// Or really, the testerapp is not meant to run in Release anyway.

	rldx::DxResourceManager::SetAssetFetchCallback(&DEBUG_Callback_FileGetter); // TODO: in release, this isn't set, for complex reasons

	auto ptestData = &test_app_data::testData_WH3_VMD_brt_ch_king_louen;
	auto ptestData2 = &test_app_data::testData_WH3_WSMODEL_2;

	auto qAssetPath = QString::fromStdWString(ptestData->assetFolder);

	// TODO: move all the ugly stuff into a CONTROLLER, if this ever becomes a proper app
	rldx::DxResourceManager::SetGameAssetFolder(qAssetPath.toStdWString());
	QString gameIdString = QString::fromStdWString(ptestData->gameId);
	QString exeFolder = QCoreApplication::applicationDirPath();

	auto repoFolder = QString::fromStdWString(GetRepoRootFolderFromExe(exeFolder.toStdWString()));

	QString globalAssetFolder = repoFolder + QString::fromStdWString(LR"(/Rldx/Rldx/RenderResources/)");
	SetAssetFolder(&globalAssetFolder);

	QString globalLogFolder = repoFolder + QString::fromStdWString(LR"(/log/)");
	SetLogFolder(&globalLogFolder);

	return;


	m_renderWidget1 = CreateQRenderingWidget(this, &gameIdString, &DEBUG_Callback_FileGetter, nullptr);
	if (!m_renderWidget1)	return;

	auto renderWidget2 = CreateQRenderingWidget(this, &gameIdString, &DEBUG_Callback_FileGetter, nullptr);
	if (!renderWidget2)	return;

	auto renderWidget3 = CreateQRenderingWidget(this, &gameIdString, &DEBUG_Callback_FileGetter, nullptr);
	if (!renderWidget3)	return;




	ByteStream bytes(ptestData->filePath);
	QString fileName = QString::fromStdWString(bytes.GetPath().c_str());
	QByteArray qBytes((char*)bytes.GetBufferPtr(), bytes.GetBufferSize());
	QString outErrorString;

	auto dockWidget1 = CreateDockWidget(renderWidget2, "Right");
	auto dockWidget2 = CreateDockWidget(renderWidget3, "Left");
	dockWidget1->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
	dockWidget2->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);

	addDockWidget(Qt::RightDockWidgetArea, dockWidget1);
	addDockWidget(Qt::LeftDockWidgetArea, dockWidget2);

	setCentralWidget(m_renderWidget1); // add widget as the central widget in the QMainWindow 
}

void QtMainWindowView::dropEvent(QDropEvent* event)
{
	// Handle drop event (e.g., process dropped file)
	// TODO:: move this code to the CONTROLLER, emit "ItemDropped" and have the controller handle that event with this code
	const QMimeData* mimeData = event->mimeData();
	if (mimeData->hasUrls()) {
		QList<QUrl> urls = mimeData->urls();
		for (const QUrl& url : urls)
		{
			auto filePath = url.toLocalFile().toStdWString();

			utils::ByteStream bytes(filePath);
			QString fileName2 = QString::fromStdWString(filePath);
			QByteArray qBytes2((char*)bytes.GetBufferPtr(), bytes.GetBufferSize());
			QString outErrorString;

			auto idString = QString::fromStdWString(game_id_keys::KEY_WARHAMMER_3);

			auto newWidget = CreateQRenderingWidget(nullptr, &idString, &DEBUG_Callback_FileGetter, nullptr);

			newWidget->setWindowFlag(Qt::WindowType::Window, true);  // make free window

			AddNewPrimaryAsset(newWidget, &fileName2, &qBytes2, &outErrorString);


			break; // only read one file, for now
		}
	}
	event->acceptProposedAction();
}

QDockWidget* QtMainWindowView::CreateDockWidget(QWidget* renderWidget, const QString& name)
{
	auto dockWidget = new QDockWidget(name, this);
	dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
								Qt::RightDockWidgetArea);

	dockWidget->setFeatures(QDockWidget::DockWidgetFeature::DockWidgetClosable);
	dockWidget->setWidget(renderWidget);
	renderWidget->setParent(dockWidget);

	return dockWidget;
}
