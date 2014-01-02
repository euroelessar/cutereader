#include "pseudobookpageitem.h"
#include "bookpageiterator.h"
#include "saferunnable.h"

PseudoBookPageItem::PseudoBookPageItem(QQuickItem *parent) :
    BookPageItem(parent), m_calulationId(0)
{
    setFlag(ItemHasContents, false);
}

void PseudoBookPageItem::recreateSubItems()
{
}

int PseudoBookPageItem::calculateNextPage(const QVariantMap &arg)
{
    int id = ++m_calulationId;
    BookPageIterator it(this);
    it.setPosition(BookTextPosition::fromMap(arg));

    SafeRunnable::start(this, [this, it, id] () -> SafeRunnable::Handler {
        QList<BookBlock::Ptr> cache;
        auto result = it.nextPage(cache).toMap();
        return [this, id, result] () {
            emit positionCalculationReady(id, result);
        };
    });

    return id;
}

int PseudoBookPageItem::calculatePreviousPage(const QVariantMap &arg)
{
    int id = ++m_calulationId;
    BookPageIterator it(this);
    it.setPosition(BookTextPosition::fromMap(arg));

    SafeRunnable::start(this, [this, it, id] () -> SafeRunnable::Handler {
        QList<BookBlock::Ptr> cache;
        auto result = it.previousPage(cache).toMap();
        return [this, id, result] () {
            emit positionCalculationReady(id, result);
        };
    });

    return id;
}
