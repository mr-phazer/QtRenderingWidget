#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtMainWindowView.h"

class QtMainWindowView : public QMainWindow, public Ui::QtTesterAppClass
{
	Q_OBJECT

public:
	QtMainWindowView(QWidget* parent = nullptr);
	~QtMainWindowView();

	// TODO: Debugging code in here, clean up
	void InitRenderView_DEBUG();



};
