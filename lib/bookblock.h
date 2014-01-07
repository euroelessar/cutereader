#ifndef BOOKBLOCK_H
#define BOOKBLOCK_H

#include <QSharedPointer>
#include <QMutex>
#include <QPainter>
#include <QQuickItem>
#include <QTextCharFormat>

class BookBlockFactory;

class Format
{
    Q_GADGET
    Q_ENUMS(Type)
public:

    enum Type {
        InvalidType,
        Base,
        Standard,
        Strong,
        Emphasis,
        StrikeThrough,
        Sub,
        Sup,
        Title,
        Subtitle,
        InternalAnchor,
        NoteAnchor,
        ExternalAnchor,
        Epigraph,
        TextAuthor,
        Poem,
        Stanza,
        Date,
        Code,
        Cite,
        TypesCount
    };

    Format() : type(InvalidType)
    {
    }

    Format(Type type) : type(type)
    {
    }

    Format(Type type, const QString &href) : type(type), href(href)
    {
    }

    Type type;
    QString href;
};

struct FormatRange
{
    int start;
    int length;
    Format format;
};

struct BookStyle
{
    BookStyle() : generation(1), formats(Format::TypesCount), blockFormats(Format::TypesCount), colorsGeneration(1)
    {
    }

    int generation;
    QVector<QTextCharFormat> formats;
    QVector<QTextBlockFormat> blockFormats;

    int colorsGeneration;
    QVector<QColor> colors;
};

class BookBlock
{
public:
    typedef QSharedPointer<BookBlock> Ptr;
    typedef QWeakPointer<BookBlock> WeakPtr;

    BookBlock(const QSizeF &size, const QWeakPointer<BookBlockFactory> &factory);
    virtual ~BookBlock();
    BookBlock(const BookBlock &other) = delete;
    BookBlock &operator =(const BookBlock &other) = delete;

    qreal pageHeight() const;
    qreal pageWidth() const;

    struct ItemInfo {
        QString type;
        QVariantMap properties;
    };

    virtual void draw(QPainter *painter, const QPointF &position, int line) const = 0;
    virtual QList<ItemInfo> createItems(const QPointF &position, int line) const;

    struct LineInfo {
        qreal height;
        int start;
        int length;
    };

    virtual int linesCount() const = 0;
    virtual int lineForPosition(int position) const = 0;
    virtual LineInfo lineInfo(int line) const = 0;

private:
    QSizeF m_pageSize;
    QWeakPointer<BookBlockFactory> m_factory;
};

Q_DECLARE_METATYPE(QList<BookBlock::ItemInfo>)
Q_DECLARE_METATYPE(QList<BookBlock::Ptr>)

#endif // BOOKBLOCK_H
