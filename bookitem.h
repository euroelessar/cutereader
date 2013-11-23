#ifndef BOOKITEM_H
#define BOOKITEM_H

#include <QObject>
#include "booktextblock.h"

class BookItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)
public:
    explicit BookItem(QObject *parent = 0);
    
    QList<BookTextBlock::Ptr> blocks() const;
    
    QString filePath() const;
    
signals:
    void filePathChanged(const QString &filePath);
    
public slots:
    void setFilePath(const QString &filePath);
    
private:
    QList<BookTextBlock::Ptr> m_blocks;
    QString m_filePath;
};

#endif // BOOKITEM_H
