#ifndef QTZLWORKER_H
#define QTZLWORKER_H

#include <QObject>
#include <functional>

#include "../lib/bookinfo.h"

class QtZLWorker : public QObject
{
    Q_OBJECT
public:
    explicit QtZLWorker(QObject *parent = 0);

    static void createInstance();
    static void deleteInstance();
    
    void loadBooks(const std::function<void (const QList<CuteReader::BodyInfo> &)> &handler);

private:
    static QtZLWorker *self;
};

#endif // QTZLWORKER_H
