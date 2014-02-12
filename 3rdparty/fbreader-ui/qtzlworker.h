#ifndef QTZLWORKER_H
#define QTZLWORKER_H

#include <QObject>
#include <functional>
#include <QPointer>

#include "../lib/bookinfo.h"

typedef std::function<void()> QtZLWork;

class QtZLWorkerPrivate;

struct QtZLBookInfo
{
    CuteReader::BookInfo book;
    QList<CuteReader::BookTextPosition> positions;
};

class QtZLWorker : public QObject
{
    Q_OBJECT
public:
    explicit QtZLWorker(QObject *parent = 0);
    ~QtZLWorker();

    static QtZLWorker &instance();
    static void createInstance();
    static void deleteInstance();
    
    void loadBooks(QObject *object, const std::function<void (const QList<CuteReader::BookInfo> &)> &handler);
    void openBook(QObject *object, const QString &path,
                  const std::function<void (const QtZLBookInfo &)> &handler,
                  const std::function<void (const QString &)> &error);
    void savePositions(const QString &path, const QList<CuteReader::BookTextPosition> &positions);
    QImage loadCover(const QString &id);
    void renderPage(QObject *object, const QSize &size, const CuteReader::BookTextPosition &position,
                    const std::function<void (const QImage &image)> &handler);
    void findNextPage(QObject *object, const QSize &size, const CuteReader::BookTextPosition &position, int delta,
                      const std::function<void (const CuteReader::BookTextPosition &image)> &handler);

private:
    void run(QObject *object, const std::function<QtZLWork ()> &work);
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
