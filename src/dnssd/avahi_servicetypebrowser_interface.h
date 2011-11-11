/*
 * This file was generated by qdbusxml2cpp version 0.7
 *
 * qdbusxml2cpp is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef AVAHI_SERVICETYPEBROWSER_INTERFACE_H_1320956278
#define AVAHI_SERVICETYPEBROWSER_INTERFACE_H_1320956278

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface org.freedesktop.Avahi.ServiceTypeBrowser
 */
class OrgFreedesktopAvahiServiceTypeBrowserInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.freedesktop.Avahi.ServiceTypeBrowser"; }

public:
    OrgFreedesktopAvahiServiceTypeBrowserInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~OrgFreedesktopAvahiServiceTypeBrowserInterface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> Free()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Free"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void AllForNow();
    void CacheExhausted();
    void Failure(const QString &error);
    void ItemNew(int interface, int protocol, const QString &type, const QString &domain, uint flags);
    void ItemRemove(int interface, int protocol, const QString &type, const QString &domain, uint flags);
};

namespace org {
  namespace freedesktop {
    namespace Avahi {
      typedef ::OrgFreedesktopAvahiServiceTypeBrowserInterface ServiceTypeBrowser;
    }
  }
}
#endif
