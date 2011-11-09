#include "mainwindow.h"

#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include "playfield.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    :QMainWindow(parent, flags)
{
    setupUi(this);

    QToolBar *toolbar = new QToolBar(this);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QAction *action = new QAction(QIcon(":/data/new-game.png"), "New game", this);
    connect(action, SIGNAL(triggered()), SLOT(newGame()));
    toolbar->addAction(action);

    action = new QAction(QIcon(":/data/rotate.png"), "Rotate ship", this);
    connect(action, SIGNAL(triggered()), SLOT(rotate()));
    toolbar->addAction(action);

    action = new QAction(QIcon(":/data/restart-game.png"), "Restart game", this);
    connect(action, SIGNAL(triggered()), SLOT(restartGame()));
    toolbar->addAction(action);

    action = new QAction(QIcon(":/data/application-exit.png"), "Quit", this);
    connect(action, SIGNAL(triggered()), SLOT(quit()));
    toolbar->addAction(action);

    addToolBar(toolbar);

    m_main = new PlayField(this, statusBar());
    setCentralWidget(m_main);

    connect(m_main, SIGNAL(startingGame()), SLOT(startingGame()));
    newGame();
}

MainWindow::~MainWindow()
{
}

void MainWindow::startingGame()
{
    statusBar()->showMessage("Place your ships");
}

void MainWindow::newGame()
{
    m_main->newGame();
}

void MainWindow::restartGame()
{
    //FIXME
}

void MainWindow::rotate()
{
    //TODO
}

void MainWindow::quit()
{
    close();
}
