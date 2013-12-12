#ifndef BOOKTEXTBLOCKFACTORY_H
#define BOOKTEXTBLOCKFACTORY_H

#include "bookblockfactory.h"
#include "booktextblock.h"

class BookTextBlockFactory : public BookBlockFactory
{
public:
    BookTextBlockFactory(const QString &text, const QFont &font, const QList<QTextLayout::FormatRange> &formats);

    static BookBlockFactory::Ptr create(const QString &text, const QFont &font, const QList<QTextLayout::FormatRange> &formats);

protected:
    BookBlock::Ptr doCreate(const QSizeF &size);

private:
    BookTextBlockData::Ptr m_data;
};

#endif // BOOKTEXTBLOCKFACTORY_H
