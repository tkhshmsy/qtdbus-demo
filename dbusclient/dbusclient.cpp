#include "dbusclient.h"
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusReply>
#include <QDebug>

// ====
// in DBusServer side(dbusserver.cpp),
// count() is not created as method, but as property.
// we should use 'org.freedesktop.DBus.Properties.Get'
// ====
#define USE_DBUS_PROPERTY

class DBusClient::Private
{
public:
    Private();
    ~Private();
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface *iface = Q_NULLPTR;
#ifdef USE_DBUS_PROPERTY
    QDBusInterface *ifaceProperty = Q_NULLPTR;
#endif
};

DBusClient::Private::Private()
{
    iface = new QDBusInterface("local.myhost",           // from D-Bus Service
                               "/QtDBusDemo/DBusServer", // from D-Bus Path
                               "api.dbusserver",         // from D-Bus Interface
                               QDBusConnection::sessionBus());
    if (!iface) {
        qWarning() << "failed to make DBusInterface";
        exit(1);
    }
#ifdef USE_DBUS_PROPERTY
    ifaceProperty =  new QDBusInterface("local.myhost",           // from D-Bus Service
                                        "/QtDBusDemo/DBusServer", // from D-Bus Path
                                        "org.freedesktop.DBus.Properties",
                                        QDBusConnection::sessionBus());
    if (!ifaceProperty) {
        qWarning() << "failed to make DBusInterface for property";
        exit(1);
    }
#endif
}

DBusClient::Private::~Private()
{
    if (iface) {
        delete iface;
    }
#ifdef USE_DBUS_PROPERTY
    if (ifaceProperty) {
        delete ifaceProperty;
    }
#endif
}

DBusClient::DBusClient(QObject *parent)
    : QObject{parent}
    , d{new Private}
{
    // connect SIGNAL(DBusServer::countChanged)
    bool flag = d->bus.connect(d->iface->service(),
                               d->iface->path(),
                               d->iface->interface(),
                               QStringLiteral("countChanged"),
                               this,
                               SLOT(countChanged(int)));
    if (!flag) {
        qDebug() << "failed to connect DBus signal";
    }

    // for debug
    connect(this, &DBusClient::countChanged,
            [](int newCount){
        qDebug() << "countChanged" << newCount;
    });
}

DBusClient::~DBusClient()
{
    delete d;
}

int DBusClient::count() const
{
    qDebug() << __func__ ; // for debug

#ifdef USE_DBUS_PROPERTY
    // call count() via org.freedesktop.DBus.Properties.Get
    QDBusReply<QVariant> countReply = d->ifaceProperty->call("Get", "api.dbusserver", "count");
    if (!countReply.isValid()) {
        qDebug() << "invalid reply from count() with DBusProperty";
        return 0;
    }
    int count = countReply.value().toInt();
#else
    // call("count") will be failed !!,
    // because of lack of Q_INVOKABLE on count() in dbusserver.h
    QDBusReply<int> countReply = d->iface->call("count");
    if (!countReply.isValid()) {
        qDebug() << "invalid reply from count()";
        return 0;
    }
    int count = countReply.value();
#endif
    return count;
}

void DBusClient::reset()
{
    qDebug() << __func__ ; // for debug

    // call method reset()
    QDBusReply<void> resetReply = d->iface->call("reset");
    if (!resetReply.isValid()) {
        qDebug() << "invalid reply from reset()";
    }
}

void DBusClient::setCount(int newCount)
{
    qDebug() << __func__ << newCount; // for debug

    // call method setCount()
    QDBusReply<void> setCountReply = d->iface->call("setCount", newCount);
    if (!setCountReply.isValid()) {
        qDebug() << "invalid reply from setCount()";
    }
}
