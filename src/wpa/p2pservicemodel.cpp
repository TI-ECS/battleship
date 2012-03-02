#include "p2pservicemodel.h"

#include "device.h"
#include "wpa.h"

#include <QDebug>

P2PServiceModel::P2PServiceModel(QObject* parent)
    :QAbstractItemModel(parent),
     devices()
{
    wpa = new Wpa;
    wpa->setEnabled(true);

    connect(wpa, SIGNAL(deviceFound(Device&)),
            this, SLOT(deviceAppear(Device&)));

    wpa->getPeers();
}

P2PServiceModel::~P2PServiceModel()
{
    foreach (Device *d, devices)
        delete d;

    devices.clear();

    wpa->disconnect();
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
        Device *it =  devices.at(index.row());
        if (it->name().isEmpty())
            return QVariant(it->address());
        else
            return QVariant(it->name());
    } else if (role == Qt::UserRole) {
        return qVariantFromValue<Device*>(devices.at(index.row()));
    } else if (role == Qt::UserRole + 1) {
        return QVariant(devices.at(index.row())->address());
    } else if (role == Qt::UserRole + 2) {
        return QVariant(devices.at(index.row())->name());
    } else
        return QVariant();
}

QVariant P2PServiceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

void P2PServiceModel::deviceAppear(Device &device)
{
    foreach(Device *dev, devices)
        if (dev->address() == device.address())
            return;

    int pos = devices.count();
    Device *d = new Device(device);

    devices.append(d);

    emit dataChanged(this->createIndex(pos, 0),
                     this->createIndex(pos + 1, 0));
}


void P2PServiceModel::connectToItem(int pos)
{
    Device *d = devices.at(pos);

    QVariantMap properties;
    properties["address"] = d->address();
    properties["go_intent"] = 0;
    properties["join"] = true;
    properties["pincode"] = "";
    properties["method"] = "pbc";

    wpa->connectPeer(properties);
}
