#pragma once

#include <string>
#include <iostream>
#include <QMainWindow>
#include "ui_UserCenterWindow.h"
using namespace std;

class UserCenterWindow : public QMainWindow
{
	Q_OBJECT

public:
	UserCenterWindow(string id, QWidget* parent = nullptr);
	~UserCenterWindow();

private:
	string id;
	Ui::UserCenterWindowClass ui;
};