#include <QtGui/QGuiApplication>
#include <QtQuick/QtQuick>
#include "../qtquick2applicationviewer/qtquick2applicationviewer.h"
#include "../lib/bookplugin.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

#ifdef Q_OS_ANDROID
    QUrl url("assets:/qml/cutereader/qml/main.qml");
#elif defined(Q_OS_MAC)
    QUrl url("../Resources/qml/cutereader/qml/main.qml");
#else
    QUrl url("qml/cutereader/qml/main.qml");
#endif
    QQmlApplicationEngine engine;
    CuteReader::BookPlugin::registerQmlTypes(&engine);
    engine.load(url);
    return app.exec();
}
