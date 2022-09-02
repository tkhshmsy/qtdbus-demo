#include "demoserver.h"
#include <QDebug>

class DemoServer::Private
{
public:
    // if running on systemBus, needs configuration in /etc/dbus-1/system.conf
    QDBusConnection bus = QDBusConnection::sessionBus();
    int count = 0;
};

DemoServer::DemoServer(QObject *parent)
    : QObject{parent}
    , d{new Private}
{
    d->bus.registerService("local.myhost");    // from D-Bus Service
    d->bus.registerObject("/Demo/Service",     // from D-Bus Path
                          "service.interface", // from D-Bus Interface
                          this,
                          QDBusConnection::ExportAllContents);
}

int DemoServer::count() const
{
    qDebug() << __func__; // for debug
    return d->count;
}

void DemoServer::setCount(int newCount)
{
    qDebug() << __func__; // for debug
    if (d->count == newCount) {
        return;
    }
    d->count = newCount;
    emit countChanged(newCount);
}

void DemoServer::reset()
{
    qDebug() << __func__; // for debug
    setCount(0);
}
