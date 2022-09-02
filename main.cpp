#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "demoserver.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<DemoServer>("local.myhost", 1, 0, "DemoServer"); // for QML

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qtdbus-demo/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
