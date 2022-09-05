#ifndef DBUSCLIENT_H
#define DBUSCLIENT_H

#include <QtCore/QObject>

class DBusClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged)  // export
public:
    explicit DBusClient(QObject *parent = nullptr);
    ~DBusClient();

    int count() const; // NOT export
    Q_INVOKABLE void reset();  // export

public Q_SLOTS:
    void setCount(int newCount);  // export

Q_SIGNALS:
    void countChanged(int newCount);  // export

private:
    class Private;
    Private *d;
};

#endif // DBUSCLIENT_H
