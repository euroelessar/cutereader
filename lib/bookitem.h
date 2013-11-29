#ifndef BOOKITEM_H
#define BOOKITEM_H

#include <QObject>
#include "booktextblock.h"

class BookItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
public:
    explicit BookItem(QObject *parent = 0);
    
    QList<BookTextBlock::Ptr> blocks() const;
    
    QUrl source() const;

    static void registerQmlTypes();
    
signals:
    void sourceChanged(const QUrl &source);
    
public slots:
    void setSource(const QUrl &source);
    
private:
    QList<BookTextBlock::Ptr> m_blocks;
    QUrl m_source;
};

#endif // BOOKITEM_H
