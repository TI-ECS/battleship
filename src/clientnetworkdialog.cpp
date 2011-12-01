#include "clientnetworkdialog.h"

#include "wpa/device.h"
#include "wpa/p2pservicemodel.h"
#include "wpa/wpap2p.h"
#include "ui_clientnetworkdialog.h"

#include <QDebug>

ClientNetworkDialog::ClientNetworkDialog(QWidget *parent) :
    QDialog(parent, Qt::Dialog)
{
    setupUi(this);
    comboBox->setModel(new P2PServiceModel(this));

    connect(buttonBox, SIGNAL(accepted()), SLOT(startConnection()));
    connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));
}


ClientNetworkDialog::~ClientNetworkDialog()
{
}

void ClientNetworkDialog::startConnection()
{
    Device *device;
    device = comboBox->itemData(comboBox->currentIndex(),
                                Qt::UserRole).
        value<Device *>();

    qDebug() << "is accept called twice ?";
    port = device->number().toInt();
    // TODO: Change it to get dynamic ip
    hostname = "192.168.1.1"; 	// for now, we're harcoding the ip.
    if (device) {
        WPAp2p::connectToGroup(device->address());
        QDialog::Accepted;
    } else {
        QDialog::Rejected;
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
