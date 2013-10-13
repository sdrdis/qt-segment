#include "mainwindow.h"
#include <QApplication>
#include <QImage>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QImage im("../../../grayscale.png");

    MainWindow w;
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    
    return a.exec();
}


