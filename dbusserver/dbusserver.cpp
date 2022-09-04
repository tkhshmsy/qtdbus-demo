#include "dbusserver.h"
#include <QDebug>

class DBusServer::Private
{
public:
    // if running on systemBus, needs configuration in /etc/dbus-1/system.conf
    QDBusConnection bus = QDBusConnection::sessionBus();
    int count = 0;
};

DBusServer::DBusServer(QObject *parent)
    : QObject{parent}
    , d{new Private}
{
    d->bus.registerService("local.myhost");         // from D-Bus Service
    d->bus.registerObject("/QtDBusDemo/DBusServer", // from D-Bus Path
                          "api.dbusserver",         // from D-Bus Interface
                          this,
                          QDBusConnection::ExportAllContents);
}

DBusServer::~DBusServer()
{
    delete d;
}

int DBusServer::count() const
{
    qDebug() << __func__; // for debug

    return d->count;
}

void DBusServer::setCount(int newCount)
{
    qDebug() << __func__ << newCount; // for debug

    if (d->count == newCount) {
        return;
    }
    d->count = newCount;
    emit countChanged(newCount);
}

void DBusServer::reset()
{
    qDebug() << __func__; // for debug

    setCount(0);
}
