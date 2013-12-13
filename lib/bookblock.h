#ifndef BOOKBLOCK_H
#define BOOKBLOCK_H

#include <QSharedPointer>
#include <QMutex>
#include <QPainter>
#include <QQuickItem>

class BookBlockFactory;

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
