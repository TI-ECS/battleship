#include "mainwindow.h"

#include "playfield.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    :QMainWindow(parent, flags)
{
    setupUi(this);

    m_main = new PlayField(this, statusBar());
    setCentralWidget(m_main);
}


MainWindow::~MainWindow()
{
}
