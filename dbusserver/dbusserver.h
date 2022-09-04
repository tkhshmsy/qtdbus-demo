#ifndef DBUSSERVER_H
#define DBUSSERVER_H

#include <QObject>
#include <QDBusConnection>

class DBusServer : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Service", "local.myhost")
    Q_CLASSINFO("D-Bus Path", "/QtDBusDemo/DBusServer")
    Q_CLASSINFO("D-Bus Interface", "api.dbusserver")
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged) // export for both(QML and D-Bus)
public:
    explicit DBusServer(QObject *parent = nullptr);
    ~DBusServer();

    int count() const; // export for QML, but NOT for D-Bus
    Q_INVOKABLE void reset(); // export for both

public Q_SLOTS:
    void setCount(int newCount); // exprot for both

Q_SIGNALS:
    void countChanged(int newCount); // export for both

private:
    class Private;
    Private *d;
};

#endif // DBUSSERVER_H
