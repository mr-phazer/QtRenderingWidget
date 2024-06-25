#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtMainWindowView.h"

class QtMainWindowView : public QMainWindow, public Ui::QtTesterAppClass
{
	Q_OBJECT

public:
	QtMainWindowView(QWidget* parent = nullptr);
	~QtMainWindowView();

	void InitRenderView();
};
