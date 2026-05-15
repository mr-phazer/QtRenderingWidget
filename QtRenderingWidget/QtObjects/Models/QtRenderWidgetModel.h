#pragma once

#include <QObject>
#include <DataManager/DataLoader.h>


class QtRenderWidgetModel  : public QObject
{
	Q_OBJECT

public:
	QtRenderWidgetModel(QObject* parent, CallBackFuncPtr callFuncPtr);
	~QtRenderWidgetModel();

	const DataLoader& GetDataLoader() const;

private:

private:
	DataLoader  m_dataLoader;
};

