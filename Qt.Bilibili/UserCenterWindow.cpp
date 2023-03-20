#include "UserCenterWindow.h"

UserCenterWindow::UserCenterWindow(string id, QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->id = id;
}

UserCenterWindow::~UserCenterWindow()
{}
