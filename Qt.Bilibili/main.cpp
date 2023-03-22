#include "h/MainWindow.h"
#include <QSharedMemory>
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QSharedMemory sm("bilibili_qt");
    if (sm.attach())
        return 0;
    sm.create(1);
    MainWindow w;
    w.show();
    return a.exec();
}