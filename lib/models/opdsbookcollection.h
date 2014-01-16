#ifndef OPDSBOOKCOLLECTION_H
#define OPDSBOOKCOLLECTION_H

#include <QObject>
#include <QQmlParserStatus>

namespace CuteReader {

class OpdsBookCollection : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
public:
    explicit OpdsBookCollection(QObject *parent = 0);

    void classBegin();
    void componentComplete();
};

} //namespace CuteReader

#endif // OPDSBOOKCOLLECTION_H
