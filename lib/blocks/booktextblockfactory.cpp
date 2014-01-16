#include "booktextblockfactory.h"
#include "texthyphenator.h"

namespace CuteReader {

BookTextBlockFactory::BookTextBlockFactory(const QString &text, const QList<FormatRange> &formats)
    : m_data(BookTextBlockData::Ptr::create())
{
    m_data->text = text;
    m_data->formats = formats;

    m_data->text.replace(QLatin1Char('\n'), QChar::LineSeparator);

    TextHyphenator(QStringLiteral("ru")).hyphenate(m_data->text, m_data->formats);
}

BookBlockFactory::Ptr BookTextBlockFactory::create(const QString &text, const QList<FormatRange> &formats)
{
    auto result = QSharedPointer<BookTextBlockFactory>::create(text, formats);
    result->m_pointer = result.toWeakRef();
    return result;
}

BookBlock::Ptr BookTextBlockFactory::doCreate(const QSizeF &size, const BookStyle &style)
{
    return QSharedPointer<BookTextBlock>::create(m_data, size, style, m_pointer);
}

} //namespace CuteReader
