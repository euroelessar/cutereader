#ifndef QTZLWORKER_H
#define QTZLWORKER_H

#include <QObject>
#include <functional>
#include <QPointer>

#include "../lib/bookinfo.h"

typedef std::function<void()> QtZLWork;

class QtZLWorker : public QObject
{
    Q_OBJECT
public:
    explicit QtZLWorker(QObject *parent = 0);

    static QtZLWorker &instance();
    static void createInstance();
    static void deleteInstance();
    
    void loadBooks(QObject *object, const std::function<void (const QList<CuteReader::BookInfo> &)> &handler);
    QImage loadCover(const QString &id);

private:
    void run(QObject *object, const std::function<QtZLWork ()> &work);
    Q_INVOKABLE void runInternal(const QtZLWork &work);
    
    static QtZLWorker *self;
    static const QMetaMethod helperMethod;
    static const QMetaMethod selfMethod;
};

class QtZLWorkerHelper : public QObject
{
    Q_OBJECT
public:
    QtZLWorkerHelper(QObject *object);
    
    Q_INVOKABLE void run(const QtZLWork &work);
    
private:
    QPointer<QObject> m_guard;
};

Q_DECLARE_METATYPE(QtZLWork)

#endif // QTZLWORKER_H
