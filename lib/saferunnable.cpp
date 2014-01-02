#include "saferunnable.h"
#include <QThreadPool>

SafeRunnable::SafeRunnable(QObject *target, const Task &task)
    : m_target(target), m_task(task)
{
    setAutoDelete(false);
}

void SafeRunnable::start(QObject *target, const SafeRunnable::Task &task)
{
    SafeRunnable *runnable = new SafeRunnable(target, task);
    QThreadPool::globalInstance()->start(runnable);
}

void SafeRunnable::run()
{
    m_handler = m_task();

    QMetaObject::invokeMethod(this, "runHandler", Qt::QueuedConnection);
}

void SafeRunnable::runHandler()
{
    if (m_target)
        m_handler();

    deleteLater();
}
