#include "bookplugin.h"
#include <QMimeDatabase>
#include "bookpageitem.h"
#include "pseudobookpageitem.h"
#include "providers/fb2imageprovider.h"
#include "models/localbookcollection.h"
#include "models/opdsentryitem.h"
#include "fontlistmodel.h"
#include "config.h"

BookPlugin::BookPlugin(QObject *parent) :
    QObject(parent)
{
}

void BookPlugin::registerQmlTypes(QQmlEngine *engine)
{
    QMimeDatabase dataBase;
    Q_UNUSED(dataBase.allMimeTypes());
    qRegisterMetaType<BookInfo>();
    qRegisterMetaType<QList<BookInfo>>();
    engine->addImageProvider("fb2", new FB2ImageProvider);
    qmlRegisterUncreatableType<BookInfoItem>("org.qutim", 0, 3, "BookInfo", "This object is always Book property");
    qmlRegisterType<BookItem>("org.qutim", 0, 3, "Book");
    qmlRegisterUncreatableType<BookStyleItem>("org.qutim", 0, 3, "BookStyle", "This object is always Book property");
    qmlRegisterUncreatableType<BookTextStyleItem>("org.qutim", 0, 3, "BookTextStyle", "This object is always Book property");
    qmlRegisterType<BookPageItem>("org.qutim", 0, 3, "BaseBookPage");
    qmlRegisterType<PseudoBookPageItem>("org.qutim", 0, 3, "PseudoBookPage");
    qmlRegisterType<LocalBookCollection>("org.qutim", 0, 3, "LocalBookCollection");
    qmlRegisterType<BookFrontModel>("org.qutim", 0, 3, "FrontModel");
    qmlRegisterType<OpdsEntryItem>("org.qutim", 0, 3, "OpdsEntry");
    qmlRegisterType<FontListModel>("org.qutim", 0, 3, "FontListModel");
    qmlRegisterType<Config>("org.qutim", 0, 3, "Config");
}
