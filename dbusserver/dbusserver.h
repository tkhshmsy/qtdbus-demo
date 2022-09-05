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
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged) // export
public:
    explicit DBusServer(QObject *parent = nullptr);
    ~DBusServer();

    Q_INVOKABLE int count() const; // NOT export
    Q_INVOKABLE void reset(); // export

public Q_SLOTS:
    void setCount(int newCount); // exprot

Q_SIGNALS:
    void countChanged(int newCount); // export

private:
    class Private;
    Private *d;
};

#endif // DBUSSERVER_H
