#include "mainwindow.h"
#include <QApplication>
#include <pfsserver.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PFSServer w;
    w.setWindowTitle("server");
    w.show();

    MainWindow c;
    c.setWindowTitle("client");
    c.show();

    return a.exec();
}
