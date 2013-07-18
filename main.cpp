#include <QtGui/QApplication>
#include <Ivy/ivy.h>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    IvySendMsg("%s","Manish");
    w.show();
    return a.exec();
}
