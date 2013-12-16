#include "booktextblockfactory.h"

BookTextBlockFactory::BookTextBlockFactory(const QString &text, const QFont &font, const QList<FormatRange> &formats)
    : m_data(BookTextBlockData::Ptr::create())
{
    m_data->text = text;
    m_data->font = font;
    m_data->formats = formats;
}

BookBlockFactory::Ptr BookTextBlockFactory::create(const QString &text, const QFont &font, const QList<FormatRange> &formats)
{
    QFont tmp = font;
    tmp.setFamily("Ubuntu");
    auto result = QSharedPointer<BookTextBlockFactory>::create(text, tmp, formats);
    result->m_pointer = result.toWeakRef();
    return result;
}

BookBlock::Ptr BookTextBlockFactory::doCreate(const QSizeF &size, const BookStyle &style)
{
    return QSharedPointer<BookTextBlock>::create(m_data, size, style, m_pointer);
}
