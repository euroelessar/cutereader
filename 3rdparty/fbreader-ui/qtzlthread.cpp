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

void contentsGames()
{
    ZLFile file("/home/elessar/gitgames/cutereader/books/the_three_musketeers.fb2");
    shared_ptr<FormatPlugin> plugin = PluginCollection::Instance().plugin(file, false);
    qDebug() << file.path().c_str() << plugin.isNull();

    shared_ptr<Book> book = Book::loadFromFile(file);
    qDebug() << book->title().c_str() << book->encoding().c_str() << book->language().c_str();

    BookModel model(book);
    qDebug() << "model" << plugin->readModel(model);

    shared_ptr<ZLTextModel> contents = model.contentsModel();
    ContentsModel &contentsModel = static_cast<ContentsModel &>(*contents);
    for (size_t i = 0; i < contents->paragraphsNumber(); ++i) {
        ZLTextParagraph *paragraph = (*contents)[i];
        qDebug() << "========";

        for (ZLTextParagraph::Iterator it = *paragraph; !it.isEnd(); it.next()) {
            const shared_ptr<ZLTextParagraphEntry> entry = it.entry();
            switch (it.entryKind()) {
            case ZLTextParagraphEntry::TEXT_ENTRY: {
                ZLTextEntry &textEntry = static_cast<ZLTextEntry &>(*entry);
                qDebug() << it.entryKind() << QString::fromUtf8(textEntry.data(), textEntry.dataLength())
                         << contentsModel.reference(static_cast<ZLTextTreeParagraph *>(paragraph));
                break;
            }
            case ZLTextParagraphEntry::CONTROL_ENTRY: {
                ZLTextControlEntry &controlEntry = static_cast<ZLTextControlEntry &>(*entry);
                if (controlEntry.isHyperlink()) {
                    ZLTextHyperlinkControlEntry &hyperlinkEntry = static_cast<ZLTextHyperlinkControlEntry &>(*entry);
                    qDebug() << it.entryKind() << hyperlinkEntry.hyperlinkType().c_str() << hyperlinkEntry.label().c_str();
                    break;
                } else {
                    qDebug() << it.entryKind() << controlEntry.isStart();
                    break;
                }
            }
            default:
                qDebug() << it.entryKind();
            }
        }
    }
}

class QtStyle : public ZLTextStyle
{
public:
    bool isDecorated() const
    {
        return false;
    }
    const std::string &fontFamily() const
    {
        static const std::string family = qApp->font().family().toStdString();
        return family;
    }
    int fontSize() const
    {
        return 32;
        return qApp->font().pixelSize();
    }
    bool bold() const
    {
        return false;
    }
    bool italic() const
    {
        return false;
    }
    const std::string &colorStyle() const
    {
        static const std::string style = "";
        return style;
    }
    short spaceBefore(const ZLTextStyleEntry::Metrics &metrics) const
    {
        return 0;
    }
    short spaceAfter(const ZLTextStyleEntry::Metrics &metrics) const
    {
        return 0;
    }
    short lineStartIndent(const ZLTextStyleEntry::Metrics &metrics, bool rtl) const
    {
        return 0;
    }
    short lineEndIndent(const ZLTextStyleEntry::Metrics &metrics, bool rtl) const
    {
        return 0;
    }
    short firstLineIndentDelta(const ZLTextStyleEntry::Metrics &metrics) const
    {
        return 0;
    }
    int verticalShift() const
    {
        return 0;
    }
    ZLTextAlignmentType alignment() const
    {
        return ALIGN_JUSTIFY;
    }
    double lineSpace() const
    {
        return 1.;
    }
    bool allowHyphenations() const
    {
        return true;
    }
};

class QtProperties : public ZLTextArea::Properties
{

    // Properties interface
public:
    shared_ptr<ZLTextStyle> baseStyle() const
    {
        return m_style;
    }

    ZLColor color(const std::string &style) const
    {
        (void) style;
        return ZLColor(0, 0, 0);
    }

    bool isSelectionEnabled() const
    {
        return true;
    }

private:
    shared_ptr<ZLTextStyle> m_style = new QtStyle;
};

void renderingGames()
{
    ZLFile file("/home/elessar/gitgames/cutereader/books/the_three_musketeers.fb2");
    shared_ptr<FormatPlugin> plugin = PluginCollection::Instance().plugin(file, false);
    qDebug() << file.path().c_str() << plugin.isNull();

    shared_ptr<Book> book = Book::loadFromFile(file);
    qDebug() << book->title().c_str() << book->encoding().c_str() << book->language().c_str();

    BookModel model(book);
    qDebug() << "model" << plugin->readModel(model);
    ZLTextHyphenator::Instance().load(book->language());

    QImage image(600, 840, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::white);

    QtZLPaintContext context;
    context.init(image);

    QtProperties properties;

    ZLTextAreaController controller(context, properties);
    controller.setModel(model.bookTextModel());

    controller.area().setOffsets(0, 0);
    controller.area().setSize(image.width(), image.height());
    controller.preparePaintInfo();

    QVector<ZLTextWordCursor> cursors;

    for (int i = 0; i < 15; ++i) {
        const ZLTextWordCursor &cursor = controller.area().startCursor();
        cursors << cursor;
        qDebug() << QStringLiteral("page: %1, block: %2, element: %3, character: %4")
                    .arg(i + 1).arg(cursor.paragraphCursor().index()).arg(cursor.elementIndex()).arg(cursor.charIndex());

        controller.scrollPage(true, ZLTextAreaController::NO_OVERLAPPING, 1);
        controller.preparePaintInfo();

//        image.fill(Qt::white);
//        controller.area().paint();

//        QImageWriter writer(QStringLiteral("/tmp/image-%1.png").arg(i));
//        writer.write(image);
    }

    for (int i = 0; i < 15; ++i) {
        const ZLTextWordCursor cursor = cursors[i];

        controller.moveStartCursor(cursor.paragraphCursor().index(), cursor.elementIndex(), cursor.charIndex());
        controller.preparePaintInfo();

        image.fill(Qt::white);
        controller.area().paint();

        QImageWriter writer(QStringLiteral("/tmp/image-%1.png").arg(i));
        writer.write(image);
    }
//    controller.moveStartCursor(marks[3].ParagraphIndex, 0, marks[3].Offset);
//    controller.preparePaintInfo();

//    controller.area().paint();
//    context.deinit();

//    QImageWriter writer(QStringLiteral("/tmp/image.png"));
//    writer.write(image);
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
