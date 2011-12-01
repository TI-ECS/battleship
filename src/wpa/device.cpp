#include "device.h"

#include <QDebug>
#include <QString>
#include <QStringList>

Device::Device()
    :QObject(0)
{
}

Device::Device(const Device &other, QObject *parent)
    :QObject(parent)
{
    device_name = other.device_name;
    device_manufacturer = other.device_manufacturer;
    model_number = other.model_number;
    device_address = other.device_address;
}

Device::Device(const QString &address, QObject *parent)
    :QObject(parent)
{
    device_address = address;
}

Device::~Device()
{
}

void Device::setValues(const QString &values)
{
    static int count;
    QStringList fields = values.split("\n");

    count = 0;
    foreach (QString f, fields) {
        if (f.startsWith("device_name")) {
            device_name = f.mid(12); //12 == device_name=
            count++;
        } else if (f.startsWith("manufacturer")) {
            device_manufacturer = f.mid(13); //13 == manufacturer=
            count++;
        } else if (f.startsWith("model_number")) {
            model_number = f.mid(13); //13 == model_number=
            count++;
        } else if (f.startsWith("address")) {
            device_address = f.mid(8); //8 == address=
            count++;
        }

        if (count == 4)
            break;
    }

    emit valueChanged(this);
}
