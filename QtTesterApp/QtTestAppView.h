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

	// TODO: Debugging code in here, clean up
	void InitRenderView_DEBUG();

protected:
	void dragEnterEvent(QDragEnterEvent* event)  override;
	void dropEvent(QDropEvent* event) override;
};
