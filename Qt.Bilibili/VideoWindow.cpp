#include "VideoWindow.h"

VideoWindow::VideoWindow(string id, playMode mode, QWidget* parent)
	: QMainWindow(parent)
{
	this->id = id;
	this->mode = mode;
	ui.setupUi(this);
}

VideoWindow::~VideoWindow()
{

}
