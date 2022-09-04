TEMPLATE = app
TARGET = dbusserver
QT = gui quick qml dbus

HEADERS += \
    dbusserver.h

SOURCES += \
    main.cpp \
    dbusserver.cpp

RESOURCES += qml.qrc
