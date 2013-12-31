#ifndef BOOKBLOCK_H
#define BOOKBLOCK_H

#include <QSharedPointer>
#include <QMutex>
#include <QPainter>
#include <QQuickItem>
#include <QTextCharFormat>
#include <QGlyphRun>

class BookBlockFactory;

struct Format
{
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
        InternalAnchor,
        NoteAnchor,
        ExternalAnchor,
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
    BookStyle() : formats(Format::TypesCount)
    {
    }

    int generation;
    QVector<QTextCharFormat> formats;

    static BookStyle defaultStyle()
    {
        BookStyle style;
        style.generation = 1;
        style.formats[Format::Strong].setFontWeight(QFont::Bold);
        style.formats[Format::Emphasis].setFontItalic(true);
        style.formats[Format::StrikeThrough].setFontItalic(true);
        style.formats[Format::Sub].setVerticalAlignment(QTextCharFormat::AlignSubScript);
        style.formats[Format::Sup].setVerticalAlignment(QTextCharFormat::AlignSuperScript);
        style.formats[Format::Title].setFontWeight(QFont::Bold);

        QTextCharFormat &internalAnchor = style.formats[Format::InternalAnchor];
        internalAnchor.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        internalAnchor.setForeground(QColor(Qt::blue));

        QTextCharFormat &externalAnchor = style.formats[Format::ExternalAnchor];
        externalAnchor.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        externalAnchor.setForeground(QColor(Qt::red));

        QTextCharFormat &noteAnchor = style.formats[Format::NoteAnchor];
        noteAnchor.setForeground(QColor(Qt::blue));
        noteAnchor.setVerticalAlignment(QTextCharFormat::AlignSuperScript);

        return style;
    }
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
    virtual QList<QGlyphRun> glyphRuns(int line);

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
