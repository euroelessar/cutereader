#ifndef QTZLWORKER_H
#define QTZLWORKER_H

#include <QObject>
#include <functional>
#include <QPointer>

#include "qtzlguard.h"

#include "../lib/bookinfo.h"

typedef std::function<void()> QtZLWork;

class QtZLWorkerPrivate;

struct QtZLBookInfo
{
    CuteReader::BookInfo book;
    QList<CuteReader::BookTextPosition> positions;
};

typedef QWeakPointer<Implemenation::QtZLGuard> QtZLWeakGuard;

class QtZLWorker : public QObject
{
    Q_OBJECT
public:
    explicit QtZLWorker(QObject *parent = 0);
    ~QtZLWorker();

    static QtZLWorker &instance();
    static void createInstance();
    static void deleteInstance();
    
    QtZLGuard loadBooks(QObject *object, const std::function<void (const QList<CuteReader::BookInfo> &)> &handler) Q_REQUIRED_RESULT;
    QtZLGuard openBook(QObject *object, const QString &path,
                       const std::function<void (const QtZLBookInfo &)> &handler,
                       const std::function<void (const QString &)> &error) Q_REQUIRED_RESULT;
    void savePositions(const QString &path, const QList<CuteReader::BookTextPosition> &positions);
    QImage loadCover(const QString &id) Q_REQUIRED_RESULT;
    QtZLGuard renderPage(QObject *object, const QSize &size, const CuteReader::BookTextPosition &position,
                         const std::function<void (const QImage &image)> &handler) Q_REQUIRED_RESULT;
    QtZLGuard findNextPage(QObject *object, const QSize &size, const CuteReader::BookTextPosition &position, int delta,
                           const std::function<void (const CuteReader::BookTextPosition &image)> &handler) Q_REQUIRED_RESULT;

private:
    void run(QObject *object, const QtZLWeakGuard &guard, const std::function<QtZLWork ()> &work);
    Q_INVOKABLE void runInternal(const QtZLWork &work);
    
    static QtZLWorker *self;
    static const QMetaMethod helperMethod;
    static const QMetaMethod selfMethod;
    
    QScopedPointer<QtZLWorkerPrivate> m_data;
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
