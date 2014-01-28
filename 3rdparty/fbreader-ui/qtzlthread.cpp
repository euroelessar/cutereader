#include "qtzlthread.h"
#include <ZLibrary.h>
#include <ZLFile.h>
#include <QGuiApplication>
#include <QDebug>

#include "../fbreader/fbreader/src/formats/FormatPlugin.h"
#include "../fbreader/fbreader/src/library/Book.h"
#include "../fbreader/fbreader/src/library/Library.h"
#include "../fbreader/fbreader/src/bookmodel/BookModel.h"

#include <ZLTextParagraph.h>
#include <ZLTextArea.h>
#include <ZLTextAreaController.h>
#include <ZLTextHyphenator.h>
#include <QImageWriter>

#include "qtzlpaintcontext.h"

#include "qtzlworker.h"

QtZLThread::QtZLThread(QObject *parent) :
    QThread(parent), m_inited(false)
{
}

//void contentsGames()
//{
//    ZLFile file("/home/elessar/gitgames/cutereader/books/the_three_musketeers.fb2");
//    shared_ptr<FormatPlugin> plugin = PluginCollection::Instance().plugin(file, false);
//    qDebug() << file.path().c_str() << plugin.isNull();

//    shared_ptr<Book> book = Book::loadFromFile(file);
//    qDebug() << book->title().c_str() << book->encoding().c_str() << book->language().c_str();

//    BookModel model(book);
//    qDebug() << "model" << plugin->readModel(model);

//    shared_ptr<ZLTextModel> contents = model.contentsModel();
//    ContentsModel &contentsModel = static_cast<ContentsModel &>(*contents);
//    for (size_t i = 0; i < contents->paragraphsNumber(); ++i) {
//        ZLTextParagraph *paragraph = (*contents)[i];
//        qDebug() << "========";

//        for (ZLTextParagraph::Iterator it = *paragraph; !it.isEnd(); it.next()) {
//            const shared_ptr<ZLTextParagraphEntry> entry = it.entry();
//            switch (it.entryKind()) {
//            case ZLTextParagraphEntry::TEXT_ENTRY: {
//                ZLTextEntry &textEntry = static_cast<ZLTextEntry &>(*entry);
//                qDebug() << it.entryKind() << QString::fromUtf8(textEntry.data(), textEntry.dataLength())
//                         << contentsModel.reference(static_cast<ZLTextTreeParagraph *>(paragraph));
//                break;
//            }
//            case ZLTextParagraphEntry::CONTROL_ENTRY: {
//                ZLTextControlEntry &controlEntry = static_cast<ZLTextControlEntry &>(*entry);
//                if (controlEntry.isHyperlink()) {
//                    ZLTextHyperlinkControlEntry &hyperlinkEntry = static_cast<ZLTextHyperlinkControlEntry &>(*entry);
//                    qDebug() << it.entryKind() << hyperlinkEntry.hyperlinkType().c_str() << hyperlinkEntry.label().c_str();
//                    break;
//                } else {
//                    qDebug() << it.entryKind() << controlEntry.isStart();
//                    break;
//                }
//            }
//            default:
//                qDebug() << it.entryKind();
//            }
//        }
//    }
//}

void do_smth()
{
}

void QtZLThread::run()
{
    QByteArray appName = qApp->applicationName().toUtf8();

    int argc = 1;
    char *argv_[] = { appName.data() };
    char **argv = argv_;

    ZLibrary::init(argc, argv);
    
    Library::Instance().PathOption.setValue("/home/elessar/.local/share/books/");
    
    {
        QMutexLocker locker(&m_mutex);
        m_inited = true;
        m_condition.wakeAll();
    }

    do_smth();

//    const BookSet &books = Library::Instance().books();
//    qDebug() << "Books" << books.size();

    exec();
    
    ZLibrary::shutdown();
}

void QtZLThread::waitForInitialization()
{
    QMutexLocker locker(&m_mutex);
    while (!m_inited)
        m_condition.wait(&m_mutex);
}
