#include <QtGui/QGuiApplication>
#include <QtQuick/QtQuick>
#include "qtquick2applicationviewer.h"
#include "bookitem.h"
#include "bookpageitem.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    qmlRegisterType<BookItem>("org.qutim", 0, 3, "Book");
    qmlRegisterType<BookPageItem>("org.qutim", 0, 3, "BookPage");

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/cutereader/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
