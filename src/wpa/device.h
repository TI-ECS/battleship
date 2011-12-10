#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <QMetaType>
#include <QObject>

class Device : public QObject
{
Q_OBJECT

public:
    Device();
    Device(const Device &other, QObject *parent = 0);
    Device(const QString &address, QObject *parent = 0);
    Device(const QString &address, const QString &name,
           QObject *parent = 0);
    virtual ~Device();

    QString address() { return device_address; }
    QString manufacturer() { return device_manufacturer; }
    QString name() { return device_name; }
    QString number() { return model_number; }
    void setValues(const QString &values);

signals:
    void valueChanged(Device *dev);

private:
    QString device_name;
    QString device_number;
    QString device_manufacturer;
    QString device_address;
    QString model_number;
};

Q_DECLARE_METATYPE(Device *)

#endif /* _DEVICE_H_ */
