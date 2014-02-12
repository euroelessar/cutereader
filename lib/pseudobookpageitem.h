#ifndef PSEUDOBOOKPAGEITEM_H
#define PSEUDOBOOKPAGEITEM_H

#include "bookpageitem.h"
#include "../3rdparty/fbreader-ui/qtzlguard.h"

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
    Q_INVOKABLE void dismiss(int id);

signals:
    void positionCalculationReady(int calculationId, const QVariantMap &position);

private:
    int m_calulationId;
    QMap<int, QtZLGuard> m_guards;
};

} //namespace CuteReader

#endif // PSEUDOBOOKPAGEITEM_H
