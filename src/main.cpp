#include "coord.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qRegisterMetaType<Coord>("Coord");

    MainWindow w;
#ifndef DEBUG
    w.showFullScreen();
#else
    w.show();
#endif

    return app.exec();
}
