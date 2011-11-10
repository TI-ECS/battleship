#ifndef CLIENTNETWORKDIALOG_H
#define CLIENTNETWORKDIALOG_H

#include <QDialog>
#include "ui_clientnetworkdialog.h"

class ClientNetworkDialog : public QDialog, public Ui::ClientNetworkDialog
{
    struct Server {
        QString name;
        QString ip;
    };
    QList<Server> servers;
    QString ip;
    Q_OBJECT

public:
    explicit ClientNetworkDialog(QWidget *parent = 0);
    ~ClientNetworkDialog();
    QString getIp() const;

public slots:
    virtual void accept();
};

#endif // CLIENTNETWORKDIALOG_H
