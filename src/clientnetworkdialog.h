#ifndef CLIENTNETWORKDIALOG_H
#define CLIENTNETWORKDIALOG_H

#include "ui_clientnetworkdialog.h"

#include "remoteservice.h"

#include <QDialog>

namespace DNSSD {
    class ServiceBrowser;
}

class ClientNetworkDialog : public QDialog, public Ui::ClientNetworkDialog
{
    struct Server {
        QString name;
        QString ip;
    };
    DNSSD::ServiceBrowser* browser;
    QList<Server> servers;
    QString hostname;
    int port;
    Q_OBJECT

public:
    explicit ClientNetworkDialog(QWidget *parent = 0);
    ~ClientNetworkDialog();
    QString getHostName() const;
    int getPort();

public slots:
    virtual void accept();
};

#endif // CLIENTNETWORKDIALOG_H
