#include "clientnetworkdialog.h"

#include "remoteservice.h"
#include "servicebrowser.h"
#include "servicemodel.h"
#include "ui_clientnetworkdialog.h"

ClientNetworkDialog::ClientNetworkDialog(QWidget *parent) :
    QDialog(parent, Qt::Dialog)
{
    setupUi(this);

    browser = new DNSSD::ServiceBrowser("_battleship._tcp", true);
    comboBox->setModel(new DNSSD::ServiceModel(browser, this));

    connect(buttonBox, SIGNAL(accepted()), SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));
}

ClientNetworkDialog::~ClientNetworkDialog()
{
    delete browser;
}

void ClientNetworkDialog::accept()
{
    DNSSD::RemoteService::Ptr service;
    service = comboBox->itemData(comboBox->currentIndex(),
                                 DNSSD::ServiceModel::ServicePtrRole).
        value<DNSSD::RemoteService::Ptr>();

    if (service) {
        service.data()->resolve();
        hostname = service.data()->hostName().remove(".local");
        port = service.data()->port();
        QDialog::accept();
    } else {
        QDialog::reject();
    }
}

QString ClientNetworkDialog::getHostName() const
{
    if(result() == QDialog::Accepted)
        return hostname;
    return "";
}

int ClientNetworkDialog::getPort()
{
    if(result() == QDialog::Accepted)
        return port;
    return -1;
}
