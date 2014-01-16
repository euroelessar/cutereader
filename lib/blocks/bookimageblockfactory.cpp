#include "bookimageblockfactory.h"

namespace CuteReader {

BookImageBlockFactory::BookImageBlockFactory(const QUrl &source)
    : m_data(BookImageBlockData::Ptr::create())
{
    m_data->source = source;
}

BookBlockFactory::Ptr BookImageBlockFactory::create(const QUrl &source)
{
    auto result = QSharedPointer<BookImageBlockFactory>::create(source);
    result->m_pointer = result.toWeakRef();
    return result;
}

void BookImageBlockFactory::setImageSizes(const QHash<QUrl, QSize> &imageSizes)
{
    auto it = imageSizes.find(m_data->source);
    if (it != imageSizes.end())
        m_data->size = it.value();
}

BookBlock::Ptr BookImageBlockFactory::doCreate(const QSizeF &size, const BookStyle &style)
{
    (void) style;
    return QSharedPointer<BookImageBlock>::create(m_data, size, m_pointer);
}

} //namespace CuteReader
