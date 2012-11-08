#include "mainwindow.h"

#include "playfield.h"

#include <QAction>
#include <QProcess>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    :QMainWindow(parent, flags), m_started(false)
{
    setupUi(this);

    m_main = new PlayField(this, statusBar());
    setCentralWidget(m_main);

    QToolBar *toolbar = new QToolBar(this);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QAction *action = new QAction(QIcon(":/data/new-game.png"), "New game", this);
    connect(action, SIGNAL(triggered()), SLOT(newGame()));
    toolbar->addAction(action);

    action = new QAction(QIcon(":/data/rotate.png"), "Rotate ship", this);
    connect(action, SIGNAL(triggered()), m_main, SIGNAL(rotate()));
    toolbar->addAction(action);

    action = new QAction(QIcon(":/data/restart-game.png"), "Restart", this);
    connect(action, SIGNAL(triggered()), SLOT(restartGame()));
    toolbar->addAction(action);

    action = new QAction(QIcon(":/data/application-exit.png"), "Quit", this);
    connect(action, SIGNAL(triggered()), SLOT(quit()));
    toolbar->addAction(action);

    addToolBar(Qt::LeftToolBarArea, toolbar);

    connect(m_main, SIGNAL(startingGame()), SLOT(startingGame()));
    newGame();
}

MainWindow::~MainWindow()
{
}

void MainWindow::startingGame()
{
    m_started = true;
    statusBar()->showMessage(tr("Place your ships"));
}

void MainWindow::newGame()
{
    m_main->newGame();
}

void MainWindow::restartGame()
{
    if (m_started)
        m_main->restart();
}

void MainWindow::quit()
{
    QProcess::execute("/usr/bin/wifi_exit.sh");
    close();
}
