#ifndef BOOKBLOCK_H
#define BOOKBLOCK_H

#include <QSharedPointer>
#include <QMutex>
#include <QPainter>
#include <QQuickItem>

class BookBlock
{
public:
    typedef QSharedPointer<BookBlock> Ptr;

    BookBlock();
    virtual ~BookBlock();
    BookBlock(const BookBlock &other) = delete;
    BookBlock &operator =(const BookBlock &other) = delete;


    virtual qreal height() const = 0;
    void setSize(const QSizeF &size);
    qreal pageHeight() const;
    qreal pageWidth() const;

    struct ItemInfo {
        QString type;
        QVariantMap properties;
    };

    virtual void draw(QPainter *painter, const QPointF &position, int line) const = 0;
    virtual QList<ItemInfo> createItems(const QPointF &position, int line) const;

    virtual void setImageSizes(const QHash<QUrl, QSize> &imageSizes);

    struct LineInfo {
        qreal height;
        int start;
        int length;
    };

    virtual int linesCount() const = 0;
    virtual int lineForPosition(int position) = 0;
    virtual LineInfo lineInfo(int line) = 0;

protected:
    virtual void doSetSize(const QSizeF &size) = 0;

    mutable QMutex m_mutex;

private:
    QSizeF m_pageSize;
};

Q_DECLARE_METATYPE(QList<BookBlock::ItemInfo>)
Q_DECLARE_METATYPE(QList<BookBlock::Ptr>)

#endif // BOOKBLOCK_H
