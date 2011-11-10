/*
 * This file was generated by dbusxml2cpp version 0.6
 * Command line was: dbusxml2cpp -m -p avahi_entrygroup_interface /home/qba/src/kdelibs/dnssd/org.freedesktop.Avahi.EntryGroup.xml
 *
 * dbusxml2cpp is Copyright (C) 2006 Trolltech ASA. All rights reserved.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef AVAHI_ENTRYGROUP_INTERFACE_H_1175536773
#define AVAHI_ENTRYGROUP_INTERFACE_H_1175536773

#include <QtCore/QObject>
#include <QtCore/QByteRef>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include "types.h"


/*
 * Proxy class for interface org.freedesktop.Avahi.EntryGroup
 */
class OrgFreedesktopAvahiEntryGroupInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.freedesktop.Avahi.EntryGroup"; }

public:
    OrgFreedesktopAvahiEntryGroupInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~OrgFreedesktopAvahiEntryGroupInterface();

public Q_SLOTS: // METHODS
    inline QDBusReply<void> AddAddress(int interface, int protocol, uint flags, const QString &name, const QString &address)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(interface) << qVariantFromValue(protocol) << qVariantFromValue(flags) << qVariantFromValue(name) << qVariantFromValue(address);
        return callWithArgumentList(QDBus::Block, QLatin1String("AddAddress"), argumentList);
    }

    inline QDBusReply<void> AddRecord(int interface, int protocol, uint flags, const QString &name, ushort clazz, ushort type, uint ttl, const QByteArray &rdata)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(interface) << qVariantFromValue(protocol) << qVariantFromValue(flags) << qVariantFromValue(name) << qVariantFromValue(clazz) << qVariantFromValue(type) << qVariantFromValue(ttl) << qVariantFromValue(rdata);
        return callWithArgumentList(QDBus::Block, QLatin1String("AddRecord"), argumentList);
    }

    inline QDBusReply<void> AddService(int interface, int protocol, uint flags, const QString &name, const QString &type, const QString &domain, const QString &host, ushort port, const QList<QByteArray> &txt)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(interface) << qVariantFromValue(protocol) << qVariantFromValue(flags) << qVariantFromValue(name) << qVariantFromValue(type) << qVariantFromValue(domain) << qVariantFromValue(host) << qVariantFromValue(port) << qVariantFromValue(txt);
        return callWithArgumentList(QDBus::Block, QLatin1String("AddService"), argumentList);
    }

    inline QDBusReply<void> AddServiceSubtype(int interface, int protocol, uint flags, const QString &name, const QString &type, const QString &domain, const QString &subtype)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(interface) << qVariantFromValue(protocol) << qVariantFromValue(flags) << qVariantFromValue(name) << qVariantFromValue(type) << qVariantFromValue(domain) << qVariantFromValue(subtype);
        return callWithArgumentList(QDBus::Block, QLatin1String("AddServiceSubtype"), argumentList);
    }

    inline QDBusReply<void> Commit()
    {
        QList<QVariant> argumentList;
        return callWithArgumentList(QDBus::Block, QLatin1String("Commit"), argumentList);
    }

    inline QDBusReply<void> Free()
    {
        QList<QVariant> argumentList;
        return callWithArgumentList(QDBus::Block, QLatin1String("Free"), argumentList);
    }

    inline QDBusReply<int> GetState()
    {
        QList<QVariant> argumentList;
        return callWithArgumentList(QDBus::Block, QLatin1String("GetState"), argumentList);
    }

    inline QDBusReply<bool> IsEmpty()
    {
        QList<QVariant> argumentList;
        return callWithArgumentList(QDBus::Block, QLatin1String("IsEmpty"), argumentList);
    }

    inline QDBusReply<void> Reset()
    {
        QList<QVariant> argumentList;
        return callWithArgumentList(QDBus::Block, QLatin1String("Reset"), argumentList);
    }

    inline QDBusReply<void> UpdateServiceTxt(int interface, int protocol, uint flags, const QString &name, const QString &type, const QString &domain, const QList<QByteArray> &txt)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(interface) << qVariantFromValue(protocol) << qVariantFromValue(flags) << qVariantFromValue(name) << qVariantFromValue(type) << qVariantFromValue(domain) << qVariantFromValue(txt);
        return callWithArgumentList(QDBus::Block, QLatin1String("UpdateServiceTxt"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void StateChanged(int state, const QString &error);
};

namespace org {
  namespace freedesktop {
    namespace Avahi {
      typedef ::OrgFreedesktopAvahiEntryGroupInterface EntryGroup;
    }
  }
}
#endif
