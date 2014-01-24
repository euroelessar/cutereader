#include "qtzlworker.h"

QtZLWorker *QtZLWorker::self = nullptr;

QtZLWorker::QtZLWorker(QObject *parent) :
    QObject(parent)
{
}

void QtZLWorker::createInstance()
{
    self = new QtZLWorker;
}

void QtZLWorker::deleteInstance()
{
    delete self;
    self = nullptr;
}

void QtZLWorker::loadBooks(const std::function<void (const QList<CuteReader::BodyInfo> &)> &handler)
{
}
