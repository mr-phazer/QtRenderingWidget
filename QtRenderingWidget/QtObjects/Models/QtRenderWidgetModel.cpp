#include "QtRenderWidgetModel.h"

QtRenderWidgetModel::QtRenderWidgetModel(QObject* parent, CallBackFuncPtr callFuncPtr)
	: QObject(parent), m_dataLoader(callFuncPtr)
{}

QtRenderWidgetModel::~QtRenderWidgetModel()
{}

