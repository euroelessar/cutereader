#ifndef QTZLTHREAD_H
#define QTZLTHREAD_H

#include <QThread>
#include <QWaitCondition>
#include <QMutex>

class QtZLThread : public QThread
{
    Q_OBJECT
public:
    explicit QtZLThread(QObject *parent = 0);

    void run();
    
    void waitForInitialization();
    
private:
    bool m_inited;
    QMutex m_mutex;
    QWaitCondition m_condition;
};

#endif // QTZLTHREAD_H
