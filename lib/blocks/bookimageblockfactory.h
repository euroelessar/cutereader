#ifndef BOOKIMAGEBLOCKFACTORY_H
#define BOOKIMAGEBLOCKFACTORY_H

#include "../bookblockfactory.h"
#include "bookimageblock.h"

class BookImageBlockFactory : public BookBlockFactory
{
public:
    BookImageBlockFactory(const QUrl &source);

    static BookBlockFactory::Ptr create(const QUrl &source);

    void setImageSizes(const QHash<QUrl, QSize> &imageSizes);

protected:
    BookBlock::Ptr doCreate(const QSizeF &size);

private:
    BookImageBlockData::Ptr m_data;
};

#endif // BOOKIMAGEBLOCKFACTORY_H
