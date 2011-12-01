#ifndef _P2PSERVICEMODEL_H_
#define _P2PSERVICEMODEL_H_

#include <QAbstractListModel>
#include <QSharedPointer>

class Device;
class WPAp2p;

class P2PServiceModel : public QAbstractItemModel
{
Q_OBJECT

public:

    P2PServiceModel(QObject* parent = 0);
    virtual ~P2PServiceModel();

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    /* virtual int columnCount(const QModelIndex&) const; */
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex& index) const;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;

private slots:
    void deviceAppear(QSharedPointer<Device> dev);

private:
    QList<QSharedPointer<Device> > devices;
    WPAp2p *wpa;
};

#endif /* _P2PSERVICEMODEL_H_ */
