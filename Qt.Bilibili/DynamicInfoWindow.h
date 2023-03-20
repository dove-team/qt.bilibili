#pragma once

#include <string>
#include <iostream>
#include <QMainWindow>
#include "ui_DynamicInfoWindow.h"
using namespace std;

class DynamicInfoWindow : public QMainWindow
{
	Q_OBJECT

public:
	DynamicInfoWindow(string id,QWidget *parent = nullptr);
	~DynamicInfoWindow();

private:
	string id;
	Ui::DynamicInfoWindowClass ui;
};
