#include "dbusclient.h"
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusReply>
#include <QDebug>

// ====
// in DBusServer side(dbusserver.cpp),
// count() is not created as method, but as property.
// ====
#define USE_DBUS_PROPERTY

class DBusClient::Private
{
public:
    Private();
    ~Private();
    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusInterface *iface = Q_NULLPTR;
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
}

DBusClient::Private::~Private()
{
    if (iface) {
        delete iface;
    }
}

DBusClient::DBusClient(QObject *parent)
    : QObject{parent}
    , d{new Private}
{
    // connect SIGNAL(DBusServer::countChanged)
    bool flag = d->iface->connection().connect(d->iface->service(),
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
    QVariant reply = d->iface->property("count");
    int count = reply.toInt();
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
    QMetaObject::invokeMethod(d->iface, "reset");
}

void DBusClient::setCount(int newCount)
{
    qDebug() << __func__ << newCount; // for debug

#ifdef USE_DBUS_PROPERTY
    d->iface->setProperty("count", QVariant(newCount));
#else
    // call method setCount()
    QDBusReply<void> setCountReply = d->iface->call("setCount", newCount);
    if (!setCountReply.isValid()) {
        qDebug() << "invalid reply from setCount()";
    }
#endif
}
