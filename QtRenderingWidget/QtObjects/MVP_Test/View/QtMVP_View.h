#pragma once

#include <QWidget>
#include "ui_QtMVP_View.h"

class QtMVP_View : public QWidget
{
	Q_OBJECT

public:
	QtMVP_View(QWidget *parent = nullptr);
	~QtMVP_View();

private:
	Ui::QtMVP_ViewClass ui;
};
