#ifndef BOOKSETTINGSSOURCE_H
#define BOOKSETTINGSSOURCE_H

#include <QObject>
#include <QVariantList>

class BookSettingsSource : public QObject
{
    Q_OBJECT
public:
    explicit BookSettingsSource(QObject *parent = 0);

    Q_INVOKABLE QVariantList fontFamiliesList(bool hasDefault) const;
    Q_INVOKABLE QVariantList fontSizesList(bool hasDefault) const;
    Q_INVOKABLE QVariantList fontItalicList(bool hasDefault) const;
    Q_INVOKABLE QVariantList fontBoldList(bool hasDefault) const;
    Q_INVOKABLE QVariantList fontUnderlineList(bool hasDefault) const;
    Q_INVOKABLE QVariantList fontStrikeOutList(bool hasDefault) const;
    Q_INVOKABLE QVariantList fontVerticalAlignmentList(bool hasDefault) const;
    Q_INVOKABLE QVariantList fontAlignmentList(bool hasDefault) const;
    Q_INVOKABLE QVariantList marginsList(bool hasDefault) const;
    Q_INVOKABLE QVariantList textIndentList(bool hasDefault) const;
    Q_INVOKABLE QVariantList formatSettingsList(const QString &type) const;
    Q_INVOKABLE QVariantList formatsList() const;
    Q_INVOKABLE QVariantMap defaultFontSettings() const;

signals:

public slots:

};

#endif // BOOKSETTINGSSOURCE_H
