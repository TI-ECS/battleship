#include "mainwindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include "playfield.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    :QMainWindow(parent, flags)
{
    setupUi(this);

    m_main = new PlayField(this, statusBar());
    setCentralWidget(m_main);

    connect(m_main, SIGNAL(startingGame()), SLOT(startingGame()));

    m_main->newGame();
}

MainWindow::~MainWindow()
{
}

void MainWindow::startingGame()
{
    statusBar()->showMessage("Place your ships");
}
