#include "bookitem.h"
#include <QElapsedTimer>
#include <QDebug>
#include "saferunnable.h"
#include <QPointer>
#include <QMimeDatabase>
#include "formats/fb2/fb2reader.h"
#include "archivereader.h"
#include "models/frontmodel.h"
#include <QJsonDocument>
#include <QStandardPaths>
#include <QSaveFile>
#include <QDir>
#include <QCryptographicHash>

BookItem::BookItem(QObject *parent) :
    QObject(parent), m_state(Null), m_info(new BookInfoItem(this)),
    m_style(new BookStyleItem(this))
{
    connect(m_style, &BookStyleItem::changed, this, &BookItem::styleChanged);
}

QList<BookBlockFactory::Ptr> BookItem::blocks(int body) const
{
    return m_bookInfo.bodies.value(body).blocks;
}

void BookItem::setSource(const QUrl &source)
{
    if (m_source != source) {
        m_bookInfo = BookInfo();
        m_source = source;
        m_state = Loading;
        emit stateChanged(m_state);
        emit sourceChanged(source);
        emit bookDataChanged(bookData());

        SafeRunnable::start(this, [this, source] () -> SafeRunnable::Handler {
            auto send_error = [this, source] () {
                setError(source);
            };

            if (!source.isLocalFile())
                return send_error;

            QElapsedTimer timer;
            timer.start();

            ArchiveReader reader(source.toLocalFile());

            if (!reader.open())
                return send_error;

            QMimeDatabase mimeDataBase;
            QMimeType mime = mimeDataBase.mimeTypeForFileNameAndData(reader.fileName(), reader.device());
            FB2Reader fb2Reader;
            if (!fb2Reader.canRead(mime))
                return send_error;

            BookInfo book = fb2Reader.read(m_source, reader.device(), FB2Reader::All);
            return [this, source, book] () {
                setBookInfo(book);
            };
        });
    }
}

void BookItem::setConfigSource(const QUrl &configSource)
{
    if (m_configSource != configSource) {
        m_configSource = configSource;
        if (configSource.isLocalFile()) {
            Config config;
            config.loadDefaultConfig(configSource.toLocalFile());
        }
        emit configSourceChanged(configSource);
    }
}

static QDir bookmarksDir()
{
    const QDir path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
            + QStringLiteral("/cutereader/bookmarks/");

    if (!path.exists())
        path.mkpath(path.absolutePath());

    return path;
}

static QString bookmarkFilePath(const QUrl &source)
{
    QByteArray hash = QCryptographicHash::hash(source.toString().toUtf8(), QCryptographicHash::Md5);
    QString fileName = QString::fromLatin1(hash.toHex());

    return bookmarksDir().filePath(fileName);
}

static QList<BookTextPosition> loadPositions(const QUrl &source)
{
    QString filePath = bookmarkFilePath(source);

    QFile file(filePath);
    if (!file.open(QFile::ReadOnly))
        return QList<BookTextPosition>();

    QList<BookTextPosition> result;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QVariantList list = doc.toVariant().toList();
    for (const auto &item : list) {
        auto position = BookTextPosition::fromMap(item.toMap());
        if (!position)
            continue;
        result << position;
    }

    return result;
}

static void savePositions(const QUrl &source, const QList<BookTextPosition> &positions)
{
    QVariantList result;
    for (const auto &position : positions)
        result << position.toMap();

    QJsonDocument document = QJsonDocument::fromVariant(result);

    QString filePath = bookmarkFilePath(source);
    QSaveFile file(filePath);
    if (!file.open(QFile::WriteOnly))
        return;

    file.write(document.toJson());
    file.commit();
}

void BookItem::setPositions(const QVariantList &positions)
{
    QList<BookTextPosition> tmp;
    for (const QVariant &var : positions)
        tmp << BookTextPosition::fromMap(var.toMap());

    if (m_positions != tmp) {
        m_positions = tmp;
        savePositions(m_bookInfo.source, m_positions);
        emit positionsChanged(BookItem::positions());
    }
}

void BookItem::setBookInfo(const BookInfo &book)
{
    if (book.source == m_source) {
        m_bookInfo = book;
        m_state = Ready;
        m_info->setBookInfo(book);
        m_positions = loadPositions(book.source);
        emit stateChanged(m_state);
        emit bookDataChanged(bookData());
        emit positionsChanged(positions());
    }
}

void BookItem::setError(const QUrl &source)
{
    if (source == m_source) {
        m_state = Error;
        emit stateChanged(m_state);
    }
}

QUrl BookItem::source() const
{
    return m_source;
}

BookItem::State BookItem::state() const
{
    return m_state;
}

BookInfoItem *BookItem::info() const
{
    return m_info;
}

BookTextPosition BookItem::positionForId(const QString &id) const
{
    for (int i = 0; i < m_bookInfo.bodies.size(); ++i) {
        const BodyInfo &info = m_bookInfo.bodies[i];
        auto it = info.references.find(id);
        if (it != info.references.end()) {
            return BookTextPosition {
                i,
                it->block,
                it->position
            };
        }
    }

    return BookTextPosition {
        -1, -1, -1
    };
}

BookStyle BookItem::style() const
{
    return m_style->style();
}


void BookItem::classBegin()
{
}

void BookItem::componentComplete()
{
    m_style->componentComplete();
}

QUrl BookItem::configSource() const
{
    return m_configSource;
}

QVariant BookItem::bookData() const
{
    return QVariant::fromValue(m_bookInfo.toData());
}

BookStyleItem *BookItem::styleItem() const
{
    return m_style;
}

QVariantList BookItem::positions() const
{
    QVariantList result;
    for (const auto &position : m_positions)
        result << position.toMap();
    return result;
}
