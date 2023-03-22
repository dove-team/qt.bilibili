#include "../h/MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    this->setCentralWidget(ui.tabWidget);
    ui.tabWidget->resize(this->size());
}

MainWindow::~MainWindow()
{

}