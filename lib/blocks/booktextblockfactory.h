#ifndef BOOKTEXTBLOCKFACTORY_H
#define BOOKTEXTBLOCKFACTORY_H

#include "../bookblockfactory.h"
#include "booktextblock.h"

namespace CuteReader {

class BookTextBlockFactory : public BookBlockFactory
{
public:
    BookTextBlockFactory(const QString &text, const QList<FormatRange> &formats);

    static BookBlockFactory::Ptr create(const QString &text, const QList<FormatRange> &formats);

protected:
    BookBlock::Ptr doCreate(const QSizeF &size, const BookStyle &style);

private:
    BookTextBlockData::Ptr m_data;
};

} //namespace CuteReader

#endif // BOOKTEXTBLOCKFACTORY_H
