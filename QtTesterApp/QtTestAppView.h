#pragma once

#include <qevent.h>
#include <QtWidgets/QMainWindow>
#include "ui_QtMainWindowView.h"

class QtMainWindowView : public QMainWindow, public Ui::QtTesterAppClass
{
	Q_OBJECT

private:
	QWidget* m_renderWidget1;

public:
	QtMainWindowView(QWidget* parent = nullptr);
	~QtMainWindowView();


public:
	void dragEnterEvent(QDragEnterEvent* event)
	{
		event->acceptProposedAction();
	}

	void dropEvent(QDropEvent* event);


private:
	QDockWidget* CreateDockWidget(QWidget* renderWidget, const QString& name);
	// TODO: Debugging code in here, clean up
	void DEBUG_InitRenderView();

};
