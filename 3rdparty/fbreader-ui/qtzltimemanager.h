#ifndef QTZLTIMEMANAGER_H
#define QTZLTIMEMANAGER_H

#include <ZLTimeManager.h>
#include <QObject>
#include <QMap>

class QtZLTimeManager : public QObject, public ZLTimeManager
{
public:
    QtZLTimeManager();

    static void createInstance();

    void addTask(shared_ptr<ZLRunnable> task, int interval);

protected:
    void removeTaskInternal(shared_ptr<ZLRunnable> task);
    ZLTime currentTime() const;
    short hoursBySeconds(long seconds) const;
    short minutesBySeconds(long seconds) const;
    short yearBySeconds(long seconds) const;
    short monthBySeconds(long seconds) const;
    short dayOfMonthBySeconds(long seconds) const;
    void timerEvent(QTimerEvent *timer);

private:
    QMap<shared_ptr<ZLRunnable>, int> m_timers;
    QMap<int, shared_ptr<ZLRunnable>> m_tasks;
};

#endif // QTZLTIMEMANAGER_H
