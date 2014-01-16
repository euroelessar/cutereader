#ifndef BOOKIMAGEBLOCKFACTORY_H
#define BOOKIMAGEBLOCKFACTORY_H

#include "../bookblockfactory.h"
#include "bookimageblock.h"

namespace CuteReader {

class BookImageBlockFactory : public BookBlockFactory
{
public:
    BookImageBlockFactory(const QUrl &source);

    static BookBlockFactory::Ptr create(const QUrl &source);

    void setImageSizes(const QHash<QUrl, QSize> &imageSizes);

protected:
    BookBlock::Ptr doCreate(const QSizeF &size, const BookStyle &style);

private:
    BookImageBlockData::Ptr m_data;
};

} //namespace CuteReader

#endif // BOOKIMAGEBLOCKFACTORY_H
