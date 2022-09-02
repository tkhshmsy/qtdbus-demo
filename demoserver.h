#ifndef DEMOSERVER_H
#define DEMOSERVER_H

#include <QObject>
#include <QDBusConnection>

class DemoServer : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Service", "local.myhost")
    Q_CLASSINFO("D-Bus Path", "/Demo/Service")
    Q_CLASSINFO("D-Bus Interface", "service.interface")
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged) // export for both(QML and D-Bus)
public:
    explicit DemoServer(QObject *parent = nullptr);

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

#endif // DEMOSERVER_H
