#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowTitle("模型工具");
    QIcon icon(":/images/ICON.ico");
    w.setWindowIcon(icon);
    w.show();

    return a.exec();
}
