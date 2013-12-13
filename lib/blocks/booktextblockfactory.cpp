#include "booktextblockfactory.h"

BookTextBlockFactory::BookTextBlockFactory(const QString &text, const QFont &font, const QList<QTextLayout::FormatRange> &formats)
    : m_data(BookTextBlockData::Ptr::create())
{
    m_data->text = text;
    m_data->font = font;
    m_data->formats = formats;
}

BookBlockFactory::Ptr BookTextBlockFactory::create(const QString &text, const QFont &font, const QList<QTextLayout::FormatRange> &formats)
{
    auto result = QSharedPointer<BookTextBlockFactory>::create(text, font, formats);
    result->m_pointer = result.toWeakRef();
    return result;
}

BookBlock::Ptr BookTextBlockFactory::doCreate(const QSizeF &size)
{
    return QSharedPointer<BookTextBlock>::create(m_data, size, m_pointer);
}
