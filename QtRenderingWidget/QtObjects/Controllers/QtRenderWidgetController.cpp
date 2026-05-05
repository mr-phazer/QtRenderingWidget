#include "..\Views\QtRenderWidgetView.h"
#include "QtRenderWidgetController.h"

void QtRenderWidgetController::OnKeyPressed(QKeyEvent* keyEvent)
{
	switch (keyEvent->key())
	{
		case Qt::Key_G:
		{
			auto newState =
				(view->m_upoSceneManager->GetCurrentScene()->GetGridNode()->GetDrawState() == rldx::DxBaseNode::DrawStateEnum::Draw)
				?
				rldx::DxBaseNode::DrawStateEnum::DontDraw :
				rldx::DxBaseNode::DrawStateEnum::Draw;
			view->m_upoSceneManager->GetCurrentScene()->GetGridNode()->SetDrawState(newState);
			keyEvent->accept();
		}
		break;

		case Qt::Key_A:
		{
			view->m_upoSceneManager->GetCurrentScene()->GetVmdManager().GenerateNewVariant();
		}
		break;

		case Qt::Key_Q:
		{
			auto newState =
				(view->m_upoSceneManager->GetCurrentScene()->GetVmdManager().GetDerformer()->GetDrawState() == rldx::DxBaseNode::DrawStateEnum::Draw)
				?
				rldx::DxBaseNode::DrawStateEnum::DontDraw :
				rldx::DxBaseNode::DrawStateEnum::Draw;
			view->m_upoSceneManager->GetCurrentScene()->GetVmdManager().GetDerformer()->SetDrawState(newState);
			keyEvent->accept();
		}	
	}
}

QtRenderWidgetController::QtRenderWidgetController(QtRenderWidgetView* parentAndView) : QObject(parentAndView), view(parentAndView)
{
	// connect "event observers"
	connect(view, &QtRenderWidgetView::KeyPressed, this, &QtRenderWidgetController::OnKeyPressed);
	connect(view, &QtRenderWidgetView::WindowClosing, this, &QtRenderWidgetController::OnWindowClosing);
}

void QtRenderWidgetController::ProcessMimeData(const QMimeData* mimeData)
{
	if (mimeData->hasUrls())
	{
		QList<QUrl> urls = mimeData->urls();
		for (const QUrl& url : urls)
		{
			auto filePath = url.toLocalFile().toStdWString();

			utils::ByteStream bytes(filePath);
			QString fileName2 = QString::fromStdWString(filePath);
			QByteArray qBytes2((char*)bytes.GetBufferPtr(), bytes.GetBufferSize());
			QString outErrorString;

			LoadNewAsset(view, &fileName2, &qBytes2, &outErrorString);

			break; // TODO: only read one file, for now, should we support multiple file drag+drop?			
		}
	}
}

bool QtRenderWidgetController::LoadNewAsset(QtRenderWidgetView* renderWidget, QString* assetFolder, QByteArray* assetData, QString* outErrorString)
{
	renderWidget->PauseRendering();
	auto gameIdString = renderWidget->GetGameIdString();
	auto currentScene = renderWidget->GetSceneManager()->GetCurrentScene();
	auto& resoureceManager = renderWidget->GetSceneManager()->GetResourceManager();

	try {

		utils::ByteStream fileDataStream(assetData->data(), assetData->size(), assetFolder->toStdWString());
		rldx::DxSceneCreator::AddVariantMesh(rldx::DxDeviceManager::Device(), resoureceManager, currentScene, fileDataStream, gameIdString.toStdWString());
	}
	catch (std::exception& e)
	{
#ifdef _DEBUG
		MessageBoxA(reinterpret_cast<HWND>(renderWidget->winId()), e.what(), "Error: Exception", MB_OK | MB_ICONERROR);
#endif
		* outErrorString = QString::fromStdString(std::string("Error: Excpetion: ") + e.what());

#ifdef _DEBUG
		logging::LogAction(L"Error: Excpetion: " + utils::ToWString(e.what()));
#endif
		return false;
	}

	renderWidget->ResumeRendering();

	return true;
}

void QtRenderWidgetController::OnWindowClosing()
{
	logging::LogAction(L"Destroying assets used by the scene");

	// TODO: don't use this, as it will remove assets from ALL instances
	///DxResourceManager::FreeAll();
}
