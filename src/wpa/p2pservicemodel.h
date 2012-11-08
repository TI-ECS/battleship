#ifndef _P2PSERVICEMODEL_H_
#define _P2PSERVICEMODEL_H_

#include "device.h"

#include <QAbstractListModel>
#include <QSharedPointer>

class Wpa;

class P2PServiceModel : public QAbstractItemModel
{
Q_OBJECT

public:

    P2PServiceModel(QObject* parent = 0);
    virtual ~P2PServiceModel();

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex& index) const;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;

private slots:
    void deviceAppear(Device &dev);

public slots:
    void connectToItem(int pos);

private:
    QList<Device *> devices;
    Wpa *wpa;
};

#endif /* _P2PSERVICEMODEL_H_ */
