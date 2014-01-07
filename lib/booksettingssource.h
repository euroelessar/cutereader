#ifndef BOOKSETTINGSSOURCE_H
#define BOOKSETTINGSSOURCE_H

#include <QObject>
#include <QVariantList>

class BookSettingsSource : public QObject
{
    Q_OBJECT
public:
    explicit BookSettingsSource(QObject *parent = 0);

    Q_INVOKABLE QVariantList formatsList() const;
    Q_INVOKABLE QVariantMap defaultFontSettings() const;

signals:

public slots:

};

#endif // BOOKSETTINGSSOURCE_H
