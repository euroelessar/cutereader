#ifndef BOOKSETTINGSSOURCE_H
#define BOOKSETTINGSSOURCE_H

#include <QObject>
#include <QVariantList>

namespace CuteReader {

class BookSettingsSource : public QObject
{
    Q_OBJECT
public:
    explicit BookSettingsSource(QObject *parent = 0);

    QVariantList fontFamiliesList(bool hasDefault) const;
    QVariantList fontSizesList(bool hasDefault) const;
    QVariantList fontItalicList(bool hasDefault) const;
    QVariantList fontBoldList(bool hasDefault) const;
    QVariantList fontUnderlineList(bool hasDefault) const;
    QVariantList fontStrikeOutList(bool hasDefault) const;
    QVariantList fontVerticalAlignmentList(bool hasDefault) const;
    QVariantList fontAlignmentList(bool hasDefault) const;
    QVariantList marginsList(bool hasDefault) const;
    QVariantList textIndentList(bool hasDefault) const;

    Q_INVOKABLE QVariantMap defaultFontSettings() const;
    Q_INVOKABLE QVariantList formatSettingsList(const QString &type) const;
    Q_INVOKABLE QVariantList formatsList() const;

signals:

public slots:

};

} //namespace CuteReader

#endif // BOOKSETTINGSSOURCE_H
