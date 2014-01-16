#ifndef BOOKIMAGEBLOCK_H
#define BOOKIMAGEBLOCK_H

#include "../bookblock.h"
#include <QUrl>

namespace CuteReader {

struct BookImageBlockData
{
    typedef QSharedPointer<BookImageBlockData> Ptr;

    QUrl source;
    QSize size;
};

class BookImageBlock : public BookBlock
{
public:
    typedef QSharedPointer<BookImageBlock> Ptr;

    BookImageBlock(const BookImageBlockData::Ptr &data, const QSizeF &size, const QWeakPointer<BookBlockFactory> &factory);

    template <typename... Args>
    static Ptr create(Args &&...args)
    {
        return Ptr::create(std::forward<Args>(args)...);
    }

    void draw(QPainter *painter, const QPointF &position, int line) const;
    virtual QList<ItemInfo> createItems(const QPointF &position, int line) const;

    int linesCount() const;
    int lineForPosition(int position) const;
    LineInfo lineInfo(int line) const;

protected:
    void doSetSize(const QSizeF &size);

private:
    BookImageBlockData::Ptr m_data;
    QSize m_imageSize;
};


} //namespace CuteReader

#endif // BOOKIMAGEBLOCK_H
