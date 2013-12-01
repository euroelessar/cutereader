#include <QtGui/QGuiApplication>
#include <QtQuick/QtQuick>
#include "../qtquick2applicationviewer/qtquick2applicationviewer.h"
#include "../lib/bookitem.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    
    BookItem::registerQmlTypes(viewer.engine());
    viewer.setMainQmlFile(QStringLiteral("qml/cutereader/qml/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
