#include "pseudobookpageitem.h"
#include "bookpageiterator.h"
#include "saferunnable.h"
#include "../3rdparty/fbreader-ui/qtzlworker.h"

namespace CuteReader {

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
    return calculatePage(arg, 1);
}

int PseudoBookPageItem::calculatePreviousPage(const QVariantMap &arg)
{
    return calculatePage(arg, -1);
}

int PseudoBookPageItem::calculatePage(const QVariantMap &position, int delta)
{
    int id = ++m_calulationId;
    
    QSize size(width(), height());
    QtZLGuard guard = QtZLWorker::instance().findNextPage(this, size, BookTextPosition::fromMap(position), delta,
                                        [this, id] (const BookTextPosition &result) {
        m_guards.remove(id);
        emit positionCalculationReady(id, result.toMap());
    });
    m_guards.insert(id, guard);
    
    return id;
}

void PseudoBookPageItem::dismiss(int id)
{
    m_guards.remove(id);
}

} //namespace CuteReader
