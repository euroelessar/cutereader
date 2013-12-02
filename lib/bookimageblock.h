#ifndef BOOKIMAGEBLOCK_H
#define BOOKIMAGEBLOCK_H

#include "bookblock.h"
#include <QUrl>

class BookImageBlock : public BookBlock
{
public:
    typedef QSharedPointer<BookImageBlock> Ptr;
    
    BookImageBlock(const QUrl &source);
    
    template <typename... Args>
    static Ptr create(Args &&...args)
    {
        return Ptr::create(std::forward<Args>(args)...);
    }
    
    virtual qreal height() const;
    void draw(QPainter *painter, const QPointF &position, int line) const;
    virtual QList<ItemInfo> createItems(const QPointF &position, int line) const;
    
    int linesCount() const;
    int lineForPosition(int position);
    LineInfo lineInfo(int line);
    
    virtual void setImageSizes(const QHash<QUrl, QSize> &imageSizes);
    
protected:
    void doSetSize(const QSizeF &size);
    
private:
    QUrl m_source;
    QSize m_imageSize;
    QSize m_size;
};

#endif // BOOKIMAGEBLOCK_H
