#include "QtRenderingWidget.h"

//#include "..\RenderLib\Direct3d11Device.h"

using namespace Rldx;

QtRenderingWidget::QtRenderingWidget(QWidget* parent)
	: QWidget(parent)
{
	setupUi(this);
	
	this->setWindowTitle("RenderView (Testing)");
	
	setWindowFlag(Qt::Window, true);

	show();

	auto debug_1 = 1;
}