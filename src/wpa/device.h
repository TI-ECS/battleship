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
    virtual ~Device();

    QString address() { return device_address; }
    QString manufacturer() { return device_manufacturer; }
    QString number() { return model_number; }
    QString name() { return device_name; }
    void setValues(const QString &values);

signals:
    void valueChanged(Device *dev);

private:
    QString device_name;
    QString device_manufacturer;
    QString model_number;
    QString device_address;
};

Q_DECLARE_METATYPE(Device *)

#endif /* _DEVICE_H_ */
