#ifndef CLIENTNETWORKDIALOG_H
#define CLIENTNETWORKDIALOG_H

#include "ui_clientnetworkdialog.h"

#include <QDialog>

class ClientNetworkDialog : public QDialog, public Ui::ClientNetworkDialog
{
Q_OBJECT

public:
    explicit ClientNetworkDialog(QWidget *parent = 0);
    ~ClientNetworkDialog();
    QString getHostName() const;
    int getPort();

public slots:
    virtual void startConnection();

private:
    QString hostname;
    int port;
};

#endif // CLIENTNETWORKDIALOG_H
