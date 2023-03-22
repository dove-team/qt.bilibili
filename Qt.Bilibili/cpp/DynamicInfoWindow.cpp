#include "../h/DynamicInfoWindow.h"

DynamicInfoWindow::DynamicInfoWindow(string id,QWidget *parent)
	: QMainWindow(parent)
{
	this->id = id;
	ui.setupUi(this);
}

DynamicInfoWindow::~DynamicInfoWindow()
{}
