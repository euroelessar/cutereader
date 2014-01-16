#ifndef BOOKPLUGIN_H
#define BOOKPLUGIN_H

#include <QObject>

class QQmlEngine;

namespace CuteReader {

class BookPlugin : public QObject
{
    Q_OBJECT
public:
    explicit BookPlugin(QObject *parent = 0);

    static void registerQmlTypes(QQmlEngine *engine);
};

} //namespace CuteReader

#endif // BOOKPLUGIN_H
