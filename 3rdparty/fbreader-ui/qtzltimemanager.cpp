#include "qtzltimemanager.h"
#include <ZLTime.h>
#include <QDateTime>
#include <QTimerEvent>

QtZLTimeManager::QtZLTimeManager()
{
}

void QtZLTimeManager::createInstance()
{
    ourInstance = new QtZLTimeManager;
}

void QtZLTimeManager::addTask(shared_ptr<ZLRunnable> task, int interval)
{
    removeTaskInternal(task);

    if (interval > 0 && !task.isNull()) {
        int id = startTimer(interval);
        m_timers[task] = id;
        m_tasks[id] = task;
    }
}

void QtZLTimeManager::removeTaskInternal(shared_ptr<ZLRunnable> task)
{
    auto it = m_timers.find(task);
    if (it != m_timers.end()) {
        killTimer(it.value());
        m_tasks.remove(it.value());
        m_timers.erase(it);
    }
}

ZLTime QtZLTimeManager::currentTime() const
{
    qint64 time = QDateTime::currentDateTime().toMSecsSinceEpoch();

    return ZLTime(time / 1000, time);
}

short QtZLTimeManager::hoursBySeconds(long seconds) const
{
    return QDateTime::fromTime_t(seconds).time().hour();
}

short QtZLTimeManager::minutesBySeconds(long seconds) const
{
    return QDateTime::fromTime_t(seconds).time().minute();
}

short QtZLTimeManager::yearBySeconds(long seconds) const
{
    return QDateTime::fromTime_t(seconds).date().year();
}

short QtZLTimeManager::monthBySeconds(long seconds) const
{
    return QDateTime::fromTime_t(seconds).date().month();
}

short QtZLTimeManager::dayOfMonthBySeconds(long seconds) const
{
    return QDateTime::fromTime_t(seconds).date().day();
}

void QtZLTimeManager::timerEvent(QTimerEvent *timer)
{
    QObject::timerEvent(timer);

    m_tasks[timer->timerId()]->run();
}
