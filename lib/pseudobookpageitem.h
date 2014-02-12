#ifndef PSEUDOBOOKPAGEITEM_H
#define PSEUDOBOOKPAGEITEM_H

#include "bookpageitem.h"

namespace CuteReader {

class PseudoBookPageItem : public BookPageItem
{
    Q_OBJECT
public:
    explicit PseudoBookPageItem(QQuickItem *parent = 0);

    void recreateSubItems();

    Q_INVOKABLE int calculateNextPage(const QVariantMap &position);
    Q_INVOKABLE int calculatePreviousPage(const QVariantMap &position);
    int calculatePage(const QVariantMap &position, int delta);

signals:
    void positionCalculationReady(int calculationId, const QVariantMap &position);

private:
    int m_calulationId;
};

} //namespace CuteReader

#endif // PSEUDOBOOKPAGEITEM_H
