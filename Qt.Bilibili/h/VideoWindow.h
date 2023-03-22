#pragma once

#include <string>
#include <iostream>
#include <QMainWindow>
#include "ui_VideoWindow.h"
#include "../Qt.BIlibili.Libs/enums/playMode.h"
using namespace std;

class VideoWindow : public QMainWindow
{
	Q_OBJECT

public:
	VideoWindow(string id, playMode mode, QWidget* parent = nullptr);
	~VideoWindow();

private:
	string id;
	playMode mode;
	Ui::VideoWindowClass ui;
};