#ifndef SAFERUNNABLE_H
#define SAFERUNNABLE_H

#include <QObject>
#include <QRunnable>
#include <QPointer>

class SafeRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    typedef std::function<void ()> Handler;
    typedef std::function<Handler ()> Task;

    SafeRunnable(QObject *target, const Task &task);

    static void start(QObject *target, const Task &task);

    void run();

private slots:
    void runHandler();

private:
    QPointer<QObject> m_target;
    Task m_task;
    Handler m_handler;
};

#endif // SAFERUNNABLE_H
