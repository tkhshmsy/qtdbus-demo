# QtDbus-Demo

the sample code for QtDBus, just introduction of simple usage.
this code supports both of Qt5 and Q6

## Tested Environment
```bash
$ cat /etc/lsb-release 
DISTRIB_ID=Ubuntu
DISTRIB_RELEASE=20.04
DISTRIB_CODENAME=focal
DISTRIB_DESCRIPTION="Ubuntu 20.04.5 LTS"

$ dbus-daemon --version
D-Bus Message Bus Daemon 1.12.16
Copyright (C) 2002, 2003 Red Hat, Inc., CodeFactory AB, and others
This is free software; see the source for copying conditions.
There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

```bash
$ /opt/qt.io/Qt/5.15.9/gcc_64/bin/qmake -v
QMake version 3.1
Using Qt version 5.15.9 in /opt/qt.io/Qt/5.15.9/gcc_64/lib

$ /opt/qt.io/Qt/6.3.1/gcc_64/bin/qmake -v
QMake version 3.1
Using Qt version 6.3.1 in /opt/qt.io/Qt/6.3.1/gcc_64/lib
```

## How to build
```bash
$ git clone https://github.com/tkhshmsy/qtdbus-demo
$ mkdir build
$ cd build
$ /path/to/qmake -r ../qtdbus-demo/qtdbus-demo.pro
$ make
```

you should run dbusserver before running dbusclient.
after running,
* dbusclient displays 'count' which is taken from dbusserver's 'count' property.
* Using 'reset' and 'send' on dbusclient, it will change 'count' property on dbusserver.
  * When it changes, dbuserver emits SIGNAL, and dbusclient will receive it and reaquire 'count'.
* Using 'reset' on dbusserver, it affects as same as using dbusclient's.

## Simply way to use QtDbus
### Server side
In the class declaration,
make informations of Service, Path and Interface with Q_CLASSINFO.
```cpp
    Q_OBJECT
    Q_CLASSINFO("D-Bus Service", "local.myhost")
    Q_CLASSINFO("D-Bus Path", "/QtDBusDemo/DBusServer")
    Q_CLASSINFO("D-Bus Interface", "api.dbusserver")
```
For export as property, use Q_PROPERTY.
```cpp
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged)
```
For export as method, use Q_INVOKABLE.
Even if using Q_PROPERTY, read-method will not export without Q_INVOKABLE.
```cpp
public:
    Q_INVOKABLE void reset(); // export fot D-Bus
    int count() const; // NOT export for D-Bus without Q_INVOKABLE
```

In definition, register Service and Object.
```cpp
    QDBusConnection bus = QDBusConnection::sessionBus();
    bus.registerService("local.myhost");         // from D-Bus Service
    bus.registerObject("/QtDBusDemo/DBusServer", // from D-Bus Path
                        "api.dbusserver",         // from D-Bus Interface
                        this,
                        QDBusConnection::ExportAllContents);
```

Now, ready to serve.

### Client side

#### How to use methods over D-Bus
make QDBusInterface, and access with call().
```cpp
    QDBusInterface *iface = new QDBusInterface("local.myhost",           // from D-Bus Service
                                                "/QtDBusDemo/DBusServer", // from D-Bus Path
                                                "api.dbusserver",         // from D-Bus Interface
                                                QDBusConnection::sessionBus());
    QDBusReply<int> countReply = iface->call("count");
    int count = countReply.value();
```

#### How to receive signals from D-Bus
make QDBusConnection, and use connect() as like as the old style of QObject::connection.
```cpp
    QDBusConnection bus = QDBusConnection::sessionBus();
    bus.connect("local.myhost",           // from D-Bus Service
                "/QtDBusDemo/DBusServer", // from D-Bus Path
                "api.dbusserver",         // from D-Bus Interface
                QStringLiteral("countChanged"),
                this,
                SLOT(countChanged(int)));    
```

## LICENSE
* MIT

## Author
* tkhshmsy@gmail.com
