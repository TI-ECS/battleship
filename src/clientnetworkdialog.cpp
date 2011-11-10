#include "clientnetworkdialog.h"
#include "ui_clientnetworkdialog.h"

ClientNetworkDialog::ClientNetworkDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    //FIXME Fetch list of server from avahi
    servers.append(Server{"Server 0", "127.0.0.1"});
    servers.append(Server{"Server 1", "127.0.0.1"});
    servers.append(Server{"Server 2", "127.0.0.1"});
    servers.append(Server{"Server 3", "127.0.0.1"});
    servers.append(Server{"Server 4", "127.0.0.1"});
    servers.append(Server{"Server 5", "127.0.0.1"});

    for(int i = 0; i < servers.size(); i++)
        listWidget->addItem(servers.at(i).name);

    listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    listWidget->setCurrentRow(0);

    connect(listWidget, SIGNAL(itemActivated(QListWidgetItem*)), SLOT(accept()));
    connect(buttonBox, SIGNAL(accepted()), SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));
}

ClientNetworkDialog::~ClientNetworkDialog()
{
}

void ClientNetworkDialog::accept()
{
    ip = servers.at(listWidget->currentRow()).ip;
    QDialog::accept();
}

QString ClientNetworkDialog::getIp() const
{
    if(result() == QDialog::Accepted)
        return ip;
    return "";
}
