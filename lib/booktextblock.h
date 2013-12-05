#ifndef BOOKTEXTBLOCK_H
#define BOOKTEXTBLOCK_H

#include "bookblock.h"
#include <QStringList>
#include <QTextLayout>
#include <QSharedPointer>
#include <QMutex>

class BookTextBlock : public BookBlock
{
public:
    typedef QSharedPointer<BookTextBlock> Ptr;

    BookTextBlock(const QString &text, const QFont &font, const QList<QTextLayout::FormatRange> &formats);

    template <typename... Args>
    static Ptr create(Args &&...args)
    {
        return Ptr::create(std::forward<Args>(args)...);
    }

    qreal height() const;

    void draw(QPainter *painter, const QPointF &position, int line) const;

    int linesCount() const;
    int lineForPosition(int position);
    LineInfo lineInfo(int line);

private:
    void doSetSize(const QSizeF &size);

    QTextLayout m_textLayout;

    qreal m_height;
};

#endif // BOOKTEXTBLOCK_H
