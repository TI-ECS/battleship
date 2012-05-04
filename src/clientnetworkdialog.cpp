#include "clientnetworkdialog.h"

#include "wpa/device.h"
#include "ui_clientnetworkdialog.h"

#include <QDebug>

ClientNetworkDialog::ClientNetworkDialog(QWidget *parent) :
    QDialog(parent, Qt::Dialog)
{
    setupUi(this);
    model = new P2PServiceModel(this);
    comboBox->setModel(model);

    connect(buttonBox, SIGNAL(accepted()), SLOT(startConnection()));
    connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));
}


ClientNetworkDialog::~ClientNetworkDialog()
{
}

void ClientNetworkDialog::startConnection()
{
    port = 1234;
    hostname = "192.168.0.1";
    Device *device;
    device = comboBox->itemData(comboBox->currentIndex(),
                                Qt::UserRole).value<Device *>();

    if (device)
        model->connectToItem(comboBox->currentIndex());
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
