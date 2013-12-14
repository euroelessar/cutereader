#ifndef BOOKTEXTBLOCK_H
#define BOOKTEXTBLOCK_H

#include "../bookblock.h"
#include <QStringList>
#include <QTextLayout>
#include <QSharedPointer>
#include <QMutex>

struct BookTextBlockData
{
    typedef QSharedPointer<BookTextBlockData> Ptr;

    QString text;
    QFont font;
    QList<QTextLayout::FormatRange> formats;
};

class BookTextBlock : public BookBlock
{
public:
    typedef QSharedPointer<BookTextBlock> Ptr;

    BookTextBlock(const BookTextBlockData::Ptr &data, const QSizeF &size, const QWeakPointer<BookBlockFactory> &factory);

    template <typename... Args>
    static Ptr create(Args &&...args)
    {
        return Ptr::create(std::forward<Args>(args)...);
    }

    void draw(QPainter *painter, const QPointF &position, int line) const;

    int linesCount() const;
    int lineForPosition(int position) const;
    LineInfo lineInfo(int line) const;

private:
    void buildLayout(const QSizeF &size);

    QTextLayout m_textLayout;
    qreal m_height;
};

#endif // BOOKTEXTBLOCK_H
