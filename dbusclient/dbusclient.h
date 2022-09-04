#ifndef DBUSCLIENT_H
#define DBUSCLIENT_H

#include <QtCore/QObject>

class DBusClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged)  // export for QML
public:
    explicit DBusClient(QObject *parent = nullptr);
    ~DBusClient();

    int count() const; // export for QML
    Q_INVOKABLE void reset();  // export for QML

public Q_SLOTS:
    void setCount(int newCount);  // export for QML

Q_SIGNALS:
    void countChanged(int newCount);  // export for QML

private:
    class Private;
    Private *d;
};

#endif // DBUSCLIENT_H
