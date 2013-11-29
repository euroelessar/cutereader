#include <QtGui/QGuiApplication>
#include <QtQuick/QtQuick>
#include "../qtquick2applicationviewer/qtquick2applicationviewer.h"
#include "../lib/bookitem.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    BookItem::registerQmlTypes();

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/cutereader/qml/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
