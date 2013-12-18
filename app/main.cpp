#include <QtGui/QGuiApplication>
#include <QtQuick/QtQuick>
#include "../qtquick2applicationviewer/qtquick2applicationviewer.h"
#include "../lib/bookitem.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    
    BookItem::registerQmlTypes(viewer.engine());
#ifdef Q_OS_ANDROID
    QUrl url("assets:/qml/cutereader/qml/main.qml");
#else
    QUrl url("qml/cutereader/qml/main.qml");
#endif
    QQmlApplicationEngine engine(url);
    return app.exec();
}
