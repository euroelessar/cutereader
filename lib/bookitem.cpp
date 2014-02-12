#include "bookitem.h"
#include <QElapsedTimer>
#include <QDebug>
#include "saferunnable.h"
#include "booktextsettings.h"
#include "bookstyle.h"
#include <QPointer>
#include <QMimeDatabase>
#include "archivereader.h"
#include "models/frontmodel.h"
#include <QJsonDocument>
#include <QStandardPaths>
#include <QSaveFile>
#include <QDir>
#include <QCryptographicHash>

#include "../3rdparty/fbreader-ui/qtzlworker.h"

namespace CuteReader {

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

BookItem::BookItem(QObject *parent) :
    QObject(parent), m_state(Null), m_info(new BookInfoItem(this)),
    m_colorsGeneration(0), m_style(nullptr), m_textSettings(new BookTextSettings(this))
{
    connect(m_textSettings, &BookTextSettings::changed, this, &BookItem::onTextSettingsChanged);
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
        m_positions.clear();

        emit stateChanged(m_state);
        emit sourceChanged(source);
        emit bookDataChanged(bookData());
        emit positionsChanged(positions());

        m_openGuard = QtZLWorker::instance().openBook(this, source.toLocalFile(), [this] (const QtZLBookInfo &result) {
            m_bookInfo = result.book;
            m_positions = result.positions;
            m_info->setBookInfo(m_bookInfo);
            
            m_state = Ready;
            emit stateChanged(m_state);
            emit bookDataChanged(bookData());
            emit positionsChanged(BookItem::positions());
            emit contentsChanged(contents());
        }, [this] (const QString &error) {
            qDebug("Failed to open book: \"%s\"", qPrintable(error));

            m_state = Error;
            emit stateChanged(m_state);
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

void BookItem::setPositions(const QVariantList &positions)
{
    QList<BookTextPosition> tmp;
    for (const QVariant &var : positions)
        tmp << BookTextPosition::fromMap(var.toMap());

    if (m_positions != tmp) {
        m_positions = tmp;
        if (m_bookInfo.source.isValid())
            QtZLWorker::instance().savePositions(m_bookInfo.source.toLocalFile(), m_positions);
        emit positionsChanged(BookItem::positions());
    }
}

void BookItem::setBookInfo(const BookInfo &book)
{
    if (book.source == m_source) {
        m_bookInfo = book;
        m_state = Ready;
        m_info->setBookInfo(book);
        emit stateChanged(m_state);
        emit bookDataChanged(bookData());
        emit contentsChanged(contents());
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
    BookStyle style = m_textSettings->style();
    style.colorsGeneration = m_colorsGeneration;
    if (m_style)
        style.colors = m_style->colors();

    return style;
}


void BookItem::classBegin()
{
}

void BookItem::componentComplete()
{
}

QUrl BookItem::configSource() const
{
    return m_configSource;
}

QVariant BookItem::bookData() const
{
    return QVariant::fromValue(m_bookInfo.toData());
}

static QVariantMap contentToMap(const ContentNode &node)
{
    const BookTextPosition position = {
        node.position.block >= 0 ? 0 : -1, node.position.block, node.position.position
    };
    QVariantList children;
    for (auto child : node.children)
        children << contentToMap(child);

    return {
        { QStringLiteral("title"), node.title.join(QLatin1Char(' ')) },
        { QStringLiteral("titles"), node.title },
        { QStringLiteral("position"), position.toMap() },
        { QStringLiteral("children"), children }
    };
}

QVariant BookItem::contents() const
{
    return contentToMap(m_bookInfo.contents);
}

BookStyleItem *BookItem::styleItem() const
{
    return m_style;
}

void BookItem::setStyleItem(BookStyleItem *style)
{
    if (m_style != style) {
        if (m_style)
            disconnect(m_style, nullptr, this, nullptr);
        m_style = style;

        ++m_colorsGeneration;
        connect(style, &BookStyleItem::changed, this, &BookItem::onStyleChanged);

        emit styleItemChanged(style);
        emit styleChanged(BookItem::style());
    }
}

void BookItem::onTextSettingsChanged()
{
    const auto style = BookItem::style();
    emit styleChanged(style);
    emit textSettingsChanged(style);
}

void BookItem::onStyleChanged()
{
    ++m_colorsGeneration;
    emit styleChanged(BookItem::style());
}

QVariantList BookItem::positions() const
{
    QVariantList result;
    for (const auto &position : m_positions)
        result << position.toMap();
    return result;
}

} //namespace CuteReader
