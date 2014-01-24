#include "bookplugin.h"
#include <QMimeDatabase>
#include "bookpageitem.h"
#include "pseudobookpageitem.h"
#include "providers/fb2imageprovider.h"
#include "providers/fbcoverimageprovider.h"
#include "models/localbookcollection.h"
#include "models/opdsentryitem.h"
#include "models/downloadbooklist.h"
#include "fontlistmodel.h"
#include "config.h"
#include "bookstyle.h"
#include "booktextsettings.h"
#include "booksettingssource.h"
#include "../3rdparty/fbreader-ui/qtzlthread.h"

namespace CuteReader {

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
    engine->addImageProvider("fbcover", new FBCoverImageProvider);
    qmlRegisterUncreatableType<BookInfoItem>("org.qutim", 0, 3, "BookInfo", "This object is always Book property");
    qmlRegisterType<BookItem>("org.qutim", 0, 3, "Book");
    qmlRegisterType<BookStyleItem>("org.qutim", 0, 3, "BookStyle");
    qmlRegisterType<BookPageItem>("org.qutim", 0, 3, "BaseBookPage");
    qmlRegisterType<PseudoBookPageItem>("org.qutim", 0, 3, "PseudoBookPage");
    qmlRegisterType<LocalBookCollection>("org.qutim", 0, 3, "LocalBookCollection");
    qmlRegisterType<BookFrontModel>("org.qutim", 0, 3, "FrontModel");
    qmlRegisterType<OpdsEntryItem>("org.qutim", 0, 3, "OpdsEntry");
    qmlRegisterType<FontListModel>("org.qutim", 0, 3, "FontListModel");
    qmlRegisterType<Config>("org.qutim", 0, 3, "Config");
    qmlRegisterType<BookSettingsSource>("org.qutim", 0, 3, "BookSettingsSource");
    qmlRegisterUncreatableType<BookTextSettings>("org.qutim", 0, 3, "TextSettings", "This object only provides enum values");
    qmlRegisterType<DownloadBookList>("org.qutim", 0, 3, "DownloadBookList");
    qmlRegisterUncreatableType<DownloadBookJob>("org.qutim", 0, 3, "DownloadBookJob", "This object is always DownloadBookList's child");

    QtZLThread *thread = new QtZLThread;
    thread->start(QThread::LowPriority);
    thread->waitForInitialization();
}

} //namespace CuteReader
