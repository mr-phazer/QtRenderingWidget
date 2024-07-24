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

void QtRenderWidgetController::OnWindowClosing()
{
	logging::LogAction(L"Destroying assets used by the scene");

	// TODO: don't use this, as it will remove assets from ALL instances
	///DxResourceManager::FreeAll();
}
