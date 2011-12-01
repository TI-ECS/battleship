#include "p2pservicemodel.h"

#include "device.h"
#include "wpap2p.h"

#include <QDebug>

P2PServiceModel::P2PServiceModel(QObject* parent)
    :QAbstractItemModel(parent),
     devices()
{
    wpa = new WPAp2p;
    wpa->setEnabled(true);

    connect(wpa, SIGNAL(deviceFound(QSharedPointer<Device>)),
            this, SLOT(deviceAppear(QSharedPointer<Device>)));

    wpa->start();
    wpa->scan();
}

P2PServiceModel::~P2PServiceModel()
{
    wpa->stop();
    wpa->wait(2500000);

    delete wpa;
}

int P2PServiceModel::columnCount(const QModelIndex&) const
{
    return 1;
}

int P2PServiceModel::rowCount(const QModelIndex& parent ) const
{
    return devices.count();
}

QModelIndex P2PServiceModel::parent(const QModelIndex&) const
{
    return QModelIndex();
}

QModelIndex P2PServiceModel::index(int row, int column, const QModelIndex& parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

QVariant P2PServiceModel::data(const QModelIndex& index, int role  ) const
{
    if (!index.isValid())
        return QVariant();

    int devicesSize = devices.size();
    int row = index.row();
    if ((row >= devicesSize) || (row < 0))
        return QVariant();

    if (role == Qt::DisplayRole) {
        Device *it =  devices.at(index.row()).data();
        if (it->name().isEmpty())
            return QVariant(it->address());
        else
            return QVariant(it->name());
    } else if (role == Qt::UserRole) {
        return qVariantFromValue<Device*>(devices.at(index.row()).data());
    } else if (role == Qt::UserRole + 1) {
        return QVariant(devices.at(index.row()).data()->address());
    } else if (role == Qt::UserRole + 2) {
        return QVariant(devices.at(index.row()).data()->name());
    } else
        return QVariant();
}

QVariant P2PServiceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

void P2PServiceModel::deviceAppear(QSharedPointer<Device> dev)
{
    int items = devices.count();
    foreach (QSharedPointer<Device> aux, devices)
        if (aux.data() == dev.data())
            return;

    if (dev.data()->manufacturer() == "battleship") {
        this->devices.append(dev);
        emit dataChanged(this->createIndex(items, 0),
                         this->createIndex(items + 1, 0));
    }
}
