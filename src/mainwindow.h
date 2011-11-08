#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "ui_mainwindow.h"

#include <QMainWindow>


class PlayField;

class MainWindow : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~MainWindow();

private:
    PlayField *m_main;
};

#endif /* _MAINWINDOW_H_ */
